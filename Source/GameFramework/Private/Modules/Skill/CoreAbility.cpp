#include "CoreAbility.h"
#include "CoreCharacter.h"
#include "CoreTargetType.h"
#include "CoreAbilitySystemComponent.h"
#include "ConfigTableCache.h"
#include "EffectConfigTableRow.h"
#include "SkillSetting.h"
#include "CoreCharacterStateBase.h"
#include "CoreAbilityCondition.h"
#include "SkillBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"

UCoreAbility::UCoreAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
	//默认本地预测
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	//默认为每个actor实例
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//默认ds通知input
	bReplicateInputDirectly = true;
}

FCoreGameplayEffectContainerSpec UCoreAbility::MakeEffectContainerSpecFromContainer(const FCoreGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel) {
	// First figure out our actor info
	FCoreGameplayEffectContainerSpec ReturnSpec;

    ReturnSpec.ContainerPtr = const_cast<FCoreGameplayEffectContainer*>(&Container);

	// If we have a target type, run the targeting logic. This is optional, targets can be added later
	if (Container.TargetType.Get()) {
		TArray<FHitResult> HitResults;
		TArray<AActor*> TargetActors;
		const UCoreTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
		ACoreCharacter* TargetingCharacter = Cast<ACoreCharacter>(GetAvatarActorFromActorInfo());
		ACoreCharacterStateBase* TargetingState = Cast<ACoreCharacterStateBase>(GetOwningActorFromActorInfo());
		TargetTypeCDO->GetTargets(TargetingCharacter, TargetingState, EventData, HitResults, TargetActors);
		ReturnSpec.AddTargets(HitResults, TargetActors);
	}

	const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
	auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();

    auto CoreAbilitySystemComponent = Cast<UCoreAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Ensured());

	// Build GameplayEffectSpecs for each applied effect
	for (const FEffectInfo& EffectInfo : Container.TargetGameplayEffects) {
		FEffectConfigTableRow* EffectRow = (FEffectConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(EffectDataTable, EffectInfo.EffectID);
		int EffectLevel = OverrideGameplayLevel == -1 ? EffectInfo.EffectLevel : OverrideGameplayLevel;
        auto EffectSpec = MakeOutgoingGameplayEffectSpec(EffectRow->GameplayEffectClass, EffectLevel);

        CoreAbilitySystemComponent->EffectPreAddDynMutiDelegate.Broadcast(CoreAbilitySystemComponent, EffectSpec, EffectRow->GameplayEffectClass);

		ReturnSpec.TargetGameplayEffectSpecs.Add(EffectSpec);
	}

	return ReturnSpec;
}

FCoreGameplayEffectContainerSpec UCoreAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel) {
	FCoreGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer) {
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FCoreGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UCoreAbility::ApplyEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec) {
	TArray<FActiveGameplayEffectHandle> AllEffects;

    bool NeedCheckCounter = false;
    FCoreGameplayEffectContainer* FoundContainer = ContainerSpec.ContainerPtr;
    switch (FoundContainer->CounterEnum) {
    case CoreAbilityCounterEnum::E_None:
        break;
    case CoreAbilityCounterEnum::E_Fixed:
        NeedCheckCounter = true;
        RestCounter -= FoundContainer->FixedCounter;
        break;
    case CoreAbilityCounterEnum::E_Trigger:
        NeedCheckCounter = true;
        break;
    default:
        check(false && "not support");
        break;
    }

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs) {
        auto EffectSpecHandles = K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData);
        AllEffects.Append(EffectSpecHandles);
        if (FoundContainer->FollowGAPeriod) {
            //如果跟随技能生命周期，那么要记录下
            FollowGAPeriodEffectHandles.Append(EffectSpecHandles);
        }

        //grant ability 自动激活
        if (FoundContainer->AutoActiveGrantedAbility && SpecHandle.Data.IsValid() && SpecHandle.Data->Def && SpecHandle.Data->Def->GrantedAbilities.Num() > 0) {
            auto AllTargetActors = ContainerSpec.GetAllTargetActors();
            if (AllTargetActors.Num() > 0) {
                for (const auto& TargetActor : AllTargetActors) {
                    auto TargetAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
                    if (TargetAbilitySystemComponent) {
                        for (const auto& GrantedAbility : SpecHandle.Data->Def->GrantedAbilities) {
                            TargetAbilitySystemComponent->TryActivateAbilityByClass(GrantedAbility.Ability);
                        }
                    }
                }
            }
        }
	}

    if (NeedCheckCounter && RestCounter <= 0) {
        K2_EndAbility();
    }

	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UCoreAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel) {
	FCoreGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}

bool UCoreAbility::K2_IsActive() const {
	return IsActive();
}

bool UCoreAbility::K2_IsConditionSatisfy() {
    auto AbilitySystemComponent = Cast<UCoreAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
    bool bSatisfy = true;

    if (GroupConditionConfigs.Num() > 0) {
        //对组进行逻辑运算
        TArray<CoreAbilityConditionRelationEnum> LoopGroupRelations;
        for (int GroupIndex = 0; GroupIndex < GroupConditionConfigs.Num(); ++GroupIndex) {
            LoopGroupRelations.Add(GroupConditionConfigs[GroupIndex].Relation);
        }
        FAbilityConditionNodeInfo GroupExecuteRoot = RelationsGenerate(LoopGroupRelations);

        //组的检查函数里对里面的condition构建逻辑运算
        TFunction<bool(int)> GroupConditionCheckFunction = [this, AbilitySystemComponent](int GroupIndex){
            bool bCheckConditionResult = true;

            const auto& Group = GroupConditionConfigs[GroupIndex];
            if (Group.ConditionConfigs.Num() > 0) {
                TArray<CoreAbilityConditionRelationEnum> LoopConditionRelations;
                for (int ConditionIndex = 0; ConditionIndex < Group.ConditionConfigs.Num(); ++ConditionIndex) {
                    const auto& ConditionConfig = Group.ConditionConfigs[ConditionIndex];
                    LoopConditionRelations.Add(ConditionConfig.Relation);
                }
                FAbilityConditionNodeInfo ConditionExecuteRoot = RelationsGenerate(LoopConditionRelations);

                //条件的检查函数里面，对condition cdo判断是否满足
                TFunction<bool(int)> ConditionCheckFunc = [this, GroupIndex, AbilitySystemComponent](int ConditionIndex){
                    const auto& ConditionConfig = GroupConditionConfigs[GroupIndex].ConditionConfigs[ConditionIndex];
                    auto CoreAbilityConditionCDO = ConditionConfig.Condition->GetDefaultObject<UCoreAbilityCondition>();
                    bool bValid;
                    bool bConditionSatisfy;
                    CoreAbilityConditionCDO->DoesSatisfy(AbilitySystemComponent, this, bValid, bConditionSatisfy);
                    if (!bValid || bConditionSatisfy == ConditionConfig.bNot) {
                        return false;
                    }
                    return true;
                };

                bCheckConditionResult = ExecuteConditionRelationTree(ConditionExecuteRoot, ConditionCheckFunc);
            }

            return bCheckConditionResult;
        };

        bSatisfy = ExecuteConditionRelationTree(GroupExecuteRoot, GroupConditionCheckFunction);
    }

    return bSatisfy;
}

void UCoreAbility::OnActivateNative_Implementation() {

}

void UCoreAbility::OnEndNative_Implementation() {

}

bool UCoreAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const {
    if (USkillBlueprintLibrary::IsComboAbility(this)) {
        //前置combo，不在这里检查cd
        return true;
    }
    return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UCoreAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
    if (!FMath::IsNearlyZero(LimitActiveTime)) {
        if (LimitActiveTimeHandle.IsValid()) {
            GetWorld()->GetTimerManager().ClearTimer(LimitActiveTimeHandle);
        }

        GetWorld()->GetTimerManager().SetTimer(LimitActiveTimeHandle, this, &UCoreAbility::LimitActiveTimeCallback, LimitActiveTime, false);
    }

    OnGameplayAbilityEnded.AddUObject(this, &UCoreAbility::OnAbilityEnd);

    if (bHasBlueprintActivate) {
        //子类覆写了，走子类
        // A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
        K2_ActivateAbility();
    }
    else if (bHasBlueprintActivateFromEvent) {
        //子类覆写了，走子类
        if (TriggerEventData) {
            // A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
            K2_ActivateAbilityFromEvent(*TriggerEventData);
        }
        else {
            bool bReplicateEndAbility = false;
            bool bWasCancelled = true;
            EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
        }
    }
    else {
        RestCounter = LimitActiveCounter;

        if (!K2_IsConditionSatisfy()) {
            K2_EndAbility();
            return;
        }

        if (!K2_CheckAbilityCost()) {
            K2_EndAbility();
            return;
        }

        const bool bComboAbility = USkillBlueprintLibrary::IsComboAbility(this);
        if (bComboAbility) {
            //combo要重置cd
            auto AbilitySystemComponent = Cast<UCoreAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
            AbilitySystemComponent->ClearAbilityCooldown(this);
        }

        //先检查是否满足触发条件，这里要调到父类的check
        bool CheckCooldown = UAbilitySystemGlobals::Get().ShouldIgnoreCooldowns() || Super::CheckCooldown(CurrentSpecHandle, CurrentActorInfo);
        if (!CheckCooldown) {
            K2_EndAbility();
            return;
        }

        OnActivateNative();
    }
}

void UCoreAbility::CallEndAbility() {
	K2_EndAbility();
}

void UCoreAbility::CallInputPressed(const FGameplayAbilitySpecHandle Handle) {
    K2_InputPressed();

    auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Ensured();
    auto Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);

    OnAbilityInputPressed.Broadcast(this, Spec->InputID);
}

void UCoreAbility::CallInputReleased(const FGameplayAbilitySpecHandle Handle) {
    K2_InputReleased();

    auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Ensured();
    auto Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);

    OnAbilityInputReleased.Broadcast(this, Spec->InputID);
}

void UCoreAbility::LimitActiveTimeCallback() {
    if (IsActive()) {
        K2_EndAbility();
    }
}

void UCoreAbility::OnAbilityEnd(UGameplayAbility* Ability) {
    if (LimitActiveTimeHandle.IsValid()) {
        GetWorld()->GetTimerManager().ClearTimer(LimitActiveTimeHandle);
    }

    auto AbilitySystemComponent = Cast<UCoreAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
    //处理跟随技能生命周期的effect，要移除
    for (auto& FollowGAPeriodEffectHandle : FollowGAPeriodEffectHandles) {
        if (FollowGAPeriodEffectHandle.IsValid()) {
            auto TargetAbilitySystemComponent = FollowGAPeriodEffectHandle.GetOwningAbilitySystemComponent();
            if (IsValid(TargetAbilitySystemComponent)) {
                TargetAbilitySystemComponent->RemoveActiveGameplayEffect(FollowGAPeriodEffectHandle);
            }
        }
    }
    //重置变量
    FollowGAPeriodEffectHandles.Empty();

    if (bClearCooldownOnEnd) {
        AbilitySystemComponent->ClearAbilityCooldown(this);
    }

    OnEndNative();
}

TArray<TArray<int>> UCoreAbility::SplitRelationIndexArray(CoreAbilityConditionRelationEnum Relation, const TArray<CoreAbilityConditionRelationEnum>& LoopRelations, const TArray<int>& RelationIndexs) {
    TArray<TArray<int>> Result;

    TArray<int> SplitIndexArray;
    for (int Index = 0; Index < RelationIndexs.Num(); ++Index) {
        int RealIndex = RelationIndexs[Index];
        if (SplitIndexArray.Num() == 0) {
            SplitIndexArray.Add(RealIndex);
        }
        else {
            if (LoopRelations[RealIndex] != Relation) {
                SplitIndexArray.Add(RealIndex);
            }
            else {
                Result.Add(SplitIndexArray);
                SplitIndexArray.Empty();
                SplitIndexArray.Add(RealIndex);
            }
        }
    }
    //最后loop完，把剩下的添加
    if (SplitIndexArray.Num() > 0) {
        Result.Add(SplitIndexArray);
    }

    return Result;
}

void UCoreAbility::RelationsGenerateRecursive(FAbilityConditionNodeInfo& NodeInfo, const TArray<CoreAbilityConditionRelationEnum>& RelationOrders, int NowOrderIndex, const TArray<CoreAbilityConditionRelationEnum>& LoopRelations, const TArray<int>& RelationIndexs) {
    const auto& Relation = RelationOrders[NowOrderIndex];
    NodeInfo.IsRelationNode = true;
    NodeInfo.Relation = Relation;

    if (NowOrderIndex == 0) {
        //最后一层，不需要分割出更多的运算符
        for (const auto& Index : RelationIndexs) {
            FAbilityConditionNodeInfo& ChildRef = NodeInfo.Children.Add_GetRef(FAbilityConditionNodeInfo());
            ChildRef.Index = Index;
            ChildRef.IsRelationNode = false;
        }
    }
    else {
        TArray<TArray<int>> SplitArray = SplitRelationIndexArray(Relation, LoopRelations, RelationIndexs);

        if (SplitArray.Num() == 1 && SplitArray[0].Num() == 1) {
            for (const auto& Index : SplitArray[0]) {
                FAbilityConditionNodeInfo& ChildRef = NodeInfo.Children.Add_GetRef(FAbilityConditionNodeInfo());
                ChildRef.Index = Index;
                ChildRef.IsRelationNode = false;
            }
        }
        else {
            for (int ArrayIndex = 0; ArrayIndex < SplitArray.Num(); ++ArrayIndex) {
                FAbilityConditionNodeInfo& ChildRef = NodeInfo.Children.Add_GetRef(FAbilityConditionNodeInfo());
                //继续往下一优先级递归
                RelationsGenerateRecursive(ChildRef, RelationOrders, NowOrderIndex - 1, LoopRelations, SplitArray[ArrayIndex]);
            }
        }
    }
}

UCoreAbility::FAbilityConditionNodeInfo UCoreAbility::RelationsGenerate(const TArray<CoreAbilityConditionRelationEnum>& LoopRelations) {
    FAbilityConditionNodeInfo Root;
    //逻辑运算优先级
    static const TArray<CoreAbilityConditionRelationEnum>& RelationOrders = { CoreAbilityConditionRelationEnum::E_XOR, CoreAbilityConditionRelationEnum::E_AND, CoreAbilityConditionRelationEnum::E_OR };

    TArray<int> IndexArray;
    for (int Index = 0; Index < LoopRelations.Num(); ++Index) {
        IndexArray.Add(Index);
    }

    RelationsGenerateRecursive(Root, RelationOrders, RelationOrders.Num() - 1, LoopRelations, IndexArray);

    return Root;
}

bool UCoreAbility::ExecuteConditionRelationTree(const FAbilityConditionNodeInfo& Node, const TFunction<bool(int)>& ConditionCheckFunc) {
    bool FinalResult = true;
    for (int Index = 0; Index < Node.Children.Num(); ++Index) {
        bool ChildResult = true;
        const auto& Child = Node.Children[Index];
        if (Child.IsRelationNode) {
            ChildResult = ExecuteConditionRelationTree(Child, ConditionCheckFunc);
        }
        else {
            ChildResult = ConditionCheckFunc(Child.Index);
        }
        bool NeedStopLoop = false;
        if (Index == 0) {
            FinalResult = ChildResult;
        }
        else {
            switch (Node.Relation) {
            case CoreAbilityConditionRelationEnum::E_AND:
                FinalResult = FinalResult && ChildResult;
                break;
            case CoreAbilityConditionRelationEnum::E_OR:
                FinalResult = FinalResult || ChildResult;
                break;
            case CoreAbilityConditionRelationEnum::E_XOR:
                FinalResult = FinalResult ^ ChildResult;
                break;
            default:
                check(false);
                break;
            }
        }
        switch (Node.Relation) {
        case CoreAbilityConditionRelationEnum::E_AND:
            if (!FinalResult) {
                //and运算符，一个为false，后面都不用再运算了
                NeedStopLoop = true;
            }
            break;
        case CoreAbilityConditionRelationEnum::E_OR:
            if (FinalResult) {
                //or运算符，一个为true，后面都不用再运算了
                NeedStopLoop = true;
            }
            break;
        default:
            break;
        }
        if (NeedStopLoop) {
            break;
        }
    }
    return FinalResult;
}