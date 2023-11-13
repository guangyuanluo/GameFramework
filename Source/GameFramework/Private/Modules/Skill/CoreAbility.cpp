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
#include "ConditionBlueprintLibrary.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "ConditionTriggerSystem.h"
#include "CoreCondition.h"
#include "CoreConditionProgress.h"
#include "CoreTriggerAction.h"
#include "CoreAbilityCondition.h"
#include "CoreConditionGroup.h"
#include "CoreAbilitiesGameplayEffectComponent.h"

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
		TargetTypeCDO->GetTargets(this, EventData, HitResults, TargetActors);
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

        if (EffectSpec.IsValid()) {
            CoreAbilitySystemComponent->EffectPreAddDynMutiDelegate.Broadcast(CoreAbilitySystemComponent, EffectSpec, EffectRow->GameplayEffectClass);

            ReturnSpec.TargetGameplayEffectSpecs.Add(EffectSpec);
        }
	}

	return ReturnSpec;
}

TArray<FActiveGameplayEffectHandle> UCoreAbility::ApplyEffectContainerSpec(const FCoreGameplayEffectContainerSpec& ContainerSpec) {
	TArray<FActiveGameplayEffectHandle> AllEffects;

    FCoreGameplayEffectContainer* FoundContainer = ContainerSpec.ContainerPtr;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs) {
        auto EffectSpecHandles = K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData);
        AllEffects.Append(EffectSpecHandles);
        if (FoundContainer->FollowGAPeriod) {
            //如果跟随技能生命周期，那么要记录下
            FollowGAPeriodEffectHandles.Append(EffectSpecHandles);
        }

        //grant ability 自动激活
        if (FoundContainer->AutoActiveGrantedAbility && SpecHandle.Data.IsValid() && SpecHandle.Data->Def) {
            auto GrantedGEComponent = SpecHandle.Data->Def->FindComponent<UCoreAbilitiesGameplayEffectComponent>();
            if (GrantedGEComponent) {
                const auto& GrantAbilityConfigs = GrantedGEComponent->GetGrantAbilityConfigs();
                if (GrantAbilityConfigs.Num() > 0) {
                    auto AllTargetActors = ContainerSpec.GetAllTargetActors();
                    if (AllTargetActors.Num() > 0) {
                        for (const auto& TargetActor : AllTargetActors) {
                            auto TargetAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
                            if (TargetAbilitySystemComponent) {
                                for (const auto& GrantAbilityConfig : GrantAbilityConfigs) {
                                    TargetAbilitySystemComponent->TryActivateAbilityByClass(GrantAbilityConfig.Ability);
                                }
                            }
                        }
                    }
                }
            }
        }
	}

	return AllEffects;
}

void UCoreAbility::CallEndAbility() {
    K2_EndAbility();
}

bool UCoreAbility::K2_IsActive() const {
	return IsActive();
}

bool UCoreAbility::K2_IsConditionSatisfy() {
    return UConditionBlueprintLibrary::DoesProgressesSatisfy(RequireConditionProgresses);
}

void UCoreAbility::OnActivateNative_Implementation() {
    
}

void UCoreAbility::OnEndNative_Implementation() {

}

void UCoreAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    //生成进度对象
    auto OwnerActor = ActorInfo->OwnerActor.Get();
    for (auto TriggerCondition : TriggerConditions.Conditions) {
        auto ConditionProgress = TriggerCondition->GenerateConditionProgress(OwnerActor);
        RequireConditionProgresses.Add(ConditionProgress);
    }
    SetProgressesWithAbility(RequireConditionProgresses);
    for (auto ConditionProgress : RequireConditionProgresses) {
        ConditionProgress->Initialize();
    }

    Super::OnGiveAbility(ActorInfo, Spec);

    if (TriggerWay == CoreAbilityTriggerEnum::E_Passive) {
        auto GameInstance = Cast<UCoreGameInstance>(GetWorld()->GetGameInstance());
        auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();

        FOnAllProgressesSatisfyDelegate Callback;
        Callback.BindUFunction(this, TEXT("OnPassiveConditionTriggerCallback"));
        ConditionTriggerSystem->FollowConditions(PassiveConditionHandler, RequireConditionProgresses, Callback);
    }
}

void UCoreAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) {
    if (TriggerWay == CoreAbilityTriggerEnum::E_Passive) {
        auto World = GetWorld();
        if (World) {
            auto GameInstance = Cast<UCoreGameInstance>(World->GetGameInstance());
            auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();
            if (ConditionTriggerSystem) {
                ConditionTriggerSystem->UnfollowConditions(PassiveConditionHandler);
            }
        }
    }
    //清空进度对象
    for (auto RequireConditionProgress : RequireConditionProgresses) {
        RequireConditionProgress->Uninitialize();
    }
    RequireConditionProgresses.Empty();

    Super::OnRemoveAbility(ActorInfo, Spec);
}

bool UCoreAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const {
    if (USkillBlueprintLibrary::IsComboAbility(this)) {
        //前置combo，不在这里检查cd
        return true;
    }
    return Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
}

void UCoreAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) {
    OnGameplayAbilityEndedWithData.AddUObject(this, &UCoreAbility::OnAbilityEnd);

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
        if (!K2_IsConditionSatisfy()) {
            K2_CancelAbility();
            return;
        }

        if (!K2_CheckAbilityCost()) {
            K2_CancelAbility();
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
            K2_CancelAbility();
            return;
        }

        StartConditionTriggerListen();
        if (!IsActive()) {
            return;
        }

        StartExternFinishConditionListen();
        if (!IsActive()) {
            //有可能开始就被终止了
            return;
        }
        OnActivateNative();
    }
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

void UCoreAbility::SetCurrentReceivedEventData(const FGameplayEventData& GameEventData) {
    CurrentReceivedEventData = GameEventData;
}

void UCoreAbility::OnAbilityEnd(const FAbilityEndedData& AbilityEndedData) {
    if (!AbilityEndedData.bWasCancelled) {
        OnAbilityFinish.Broadcast();
    }

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

    StopConditionTriggerListen();
    StopExternFinishConditionListen();

    OnEndNative();
}

void UCoreAbility::OnPassiveConditionTriggerCallback(FConditionTriggerHandler Handler) {
    auto AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    if (AbilitySystemComponent) {
        AbilitySystemComponent->TryActivateAbilityByClass(GetClass());
    }
}

void UCoreAbility::StartConditionTriggerListen() {
    if (ConditionActionTriggerConfigs.Num() == 0) {
        return;
    }
    auto OwnerActor = GetOwningActorFromActorInfo();
    //生成触发器的进度监听
    for (int Index = 0; Index < ConditionActionTriggerConfigs.Num(); ++Index) {
        auto& ConditionActionTriggerConfig = ConditionActionTriggerConfigs[Index];
        if (ConditionActionTriggerConfig.ExecuteActions.Actions.Num() == 0) {
            //没有执行动作，监听无意义，多半是配错了
            ensureMsgf(false, TEXT("动作配置为空，将不生效"));
            continue;
        }
        FCoreConditionActionTriggerInfo& TriggerInfo = TriggerConditionProgressInfos.AddDefaulted_GetRef();
        for (auto Condition : ConditionActionTriggerConfig.TriggerConditions.Conditions) {
            auto Progress = Condition->GenerateConditionProgress(OwnerActor);
            TriggerInfo.TriggerConditionProgresses.Add(Progress);
        }
        SetProgressesWithAbility(TriggerInfo.TriggerConditionProgresses);
        for (auto ConditionProgress : TriggerInfo.TriggerConditionProgresses) {
            ConditionProgress->Initialize();
        }
    }
    auto GameInstance = Cast<UCoreGameInstance>(GetWorld()->GetGameInstance());
    auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();
    for (auto& TriggerConditionProgressInfo : TriggerConditionProgressInfos) {
        FOnAllProgressesSatisfyDelegate Callback;
        Callback.BindUFunction(this, TEXT("OnConditionTriggerCallback"));
        ConditionTriggerSystem->FollowConditions(TriggerConditionProgressInfo.ConditionTriggerHandler, TriggerConditionProgressInfo.TriggerConditionProgresses, Callback);
        if (!IsActive()) {
            break;
        }
    }
}

void UCoreAbility::StopConditionTriggerListen() {
    if (ConditionActionTriggerConfigs.Num() == 0) {
        return;
    }
    auto GameInstance = Cast<UCoreGameInstance>(GetWorld()->GetGameInstance());
    auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();
    if (!ConditionTriggerSystem) {
        return;
    }
    for (auto& TriggerConditionProgressInfo : TriggerConditionProgressInfos) {
        ConditionTriggerSystem->UnfollowConditions(TriggerConditionProgressInfo.ConditionTriggerHandler);
        for (auto Progress : TriggerConditionProgressInfo.TriggerConditionProgresses) {
            Progress->Uninitialize();
        }
    }
    TriggerConditionProgressInfos.Empty();
}

void UCoreAbility::OnConditionTriggerCallback(FConditionTriggerHandler Handler) {
    if (!IsActive()) {
        return;
    }
    int FindTriggerIndex = INDEX_NONE;
    for (int Index = 0; Index < TriggerConditionProgressInfos.Num(); ++Index) {
        const auto& TriggerConditionProgressInfo = TriggerConditionProgressInfos[Index];
        if (TriggerConditionProgressInfo.ConditionTriggerHandler == Handler) {
            FindTriggerIndex = Index;
            break;
        }
    }
    if (FindTriggerIndex == INDEX_NONE) {
        return;
    }
    if (!CurrentReceivedEventData.ContextHandle.IsValid()) {
        CurrentReceivedEventData.ContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
    }
    FGameplayEventData ActionEventData = CurrentReceivedEventData;
    const auto& ConditionActionTriggerConfig = ConditionActionTriggerConfigs[FindTriggerIndex];
    for (auto ExecuteAction : ConditionActionTriggerConfig.ExecuteActions.Actions) {
        FLogicObjectLoadWorldScope LoadWorldScope(ExecuteAction, this);
        ExecuteAction->OnExecute(ActionEventData);
    }
}

void UCoreAbility::StartExternFinishConditionListen() {
    if (ExternFinishConditions.Conditions.Num() == 0) {
        return;
    }
    auto OwnerActor = GetOwningActorFromActorInfo();

    for (const auto& ExternFinishCondition : ExternFinishConditions.Conditions) {
        auto Progress = ExternFinishCondition->GenerateConditionProgress(OwnerActor);
        ExternFinishConditionProgresses.Add(Progress);
    }
    SetProgressesWithAbility(ExternFinishConditionProgresses);
    for (auto Progress : ExternFinishConditionProgresses) {
        Progress->Initialize();
    }
    auto GameInstance = Cast<UCoreGameInstance>(GetWorld()->GetGameInstance());
    auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();
    FOnAllProgressesSatisfyDelegate Callback;
    Callback.BindUFunction(this, TEXT("OnExternFinishTriggerCallback"));
    ConditionTriggerSystem->FollowConditions(ExternFinishConditionHandler, ExternFinishConditionProgresses, Callback);
}

void UCoreAbility::StopExternFinishConditionListen() {
    if (ExternFinishConditions.Conditions.Num() == 0) {
        return;
    }
    for (auto ExternFinishConditionProgress : ExternFinishConditionProgresses) {
        ExternFinishConditionProgress->Uninitialize();
    }
    auto GameInstance = Cast<UCoreGameInstance>(GetWorld()->GetGameInstance());
    auto ConditionTriggerSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>();
    ConditionTriggerSystem->UnfollowConditions(ExternFinishConditionHandler);
    ExternFinishConditionProgresses.Empty();
}

void UCoreAbility::OnExternFinishTriggerCallback(FConditionTriggerHandler Handler) {
    K2_EndAbility();
}

void UCoreAbility::SetProgressesWithAbility(const TArray<class UCoreConditionProgress*>& Progresses) {
    TArray<UCoreConditionProgress*> SearchProgresses;
    for (auto Progress : Progresses) {
        SearchProgresses.Add(Progress);
    }
    while (SearchProgresses.Num() > 0) {
        auto SearchProgress = SearchProgresses[SearchProgresses.Num() - 1];
        SearchProgresses.RemoveAt(SearchProgresses.Num() - 1);
        if (auto AbilityConditionProgress = Cast<UCoreAbilityConditionProgress>(SearchProgress)) {
            AbilityConditionProgress->OwnerAbility = this;
        }
        else
        {
            if (auto ConditionGroupProgress = Cast<UCoreConditionGroupProgress>(SearchProgress)) {
                TArray<UCoreConditionProgress*> ChildProgresses;
                ConditionGroupProgress->GetProgressesWithChildren(ChildProgresses);
                for (auto ChildProgress : ChildProgresses) {
                    if (ChildProgress != ConditionGroupProgress) {
                        SearchProgresses.Add(ChildProgress);
                    }
                }
            }
        }
    }
}