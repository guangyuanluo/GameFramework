#include "CoreAbility.h"
#include "CoreCharacter.h"
#include "CoreTargetType.h"
#include "CoreAbilitySystemComponent.h"
#include "ConfigTableCache.h"
#include "EffectConfigTableRow.h"
#include "SkillSetting.h"
#include "CoreCharacterStateBase.h"
#include "CoreAbilityCondition.h"

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
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
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
    for (const auto& ConditionConfig : ConditionConfigs) {
        auto CoreAbilityConditionCDO = ConditionConfig.Condition->GetDefaultObject<UCoreAbilityCondition>();
        bool bValid;
        bool bConditionSatisfy;
        CoreAbilityConditionCDO->DoesSatisfy(AbilitySystemComponent, this, bValid, bConditionSatisfy);
        if (!bValid || bConditionSatisfy == ConditionConfig.bNot) {
            bSatisfy = false;
            break;
        }
    }
    return bSatisfy;
}

void UCoreAbility::OnActivateNative_Implementation() {

}

void UCoreAbility::OnEndNative_Implementation() {

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
    for (auto Iter = EffectContainerMap.CreateConstIterator(); Iter; ++Iter) {
        if (Iter->Value.FollowGAPeriod) {
            for (auto EffectInfo : Iter->Value.TargetGameplayEffects) {
                AbilitySystemComponent->RemoveEffect(EffectInfo);
            }
        }
    }

    if (bClearCooldownOnEnd) {
        AbilitySystemComponent->ClearAbilityCooldown(this);
    }

    OnEndNative();
}