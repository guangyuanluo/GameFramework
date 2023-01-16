#include "CoreAbility.h"
#include "CoreAbilityComboExecutor.h"
#include "CoreCharacter.h"
#include "CoreTargetType.h"
#include "CoreAbilitySystemComponent.h"
#include "ConfigTableCache.h"
#include "EffectConfigTableRow.h"
#include "SkillSetting.h"
#include "CoreCharacterStateBase.h"
#include "CoreAbilityComboExecutor.h"

UCoreAbility::UCoreAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
	//默认本地预测
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	//默认为每个actor实例
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	//默认ds通知input
	bReplicateInputDirectly = true;

	ComboExecutor = UCoreAbilityComboExecutor_Default::StaticClass();
}

FCoreGameplayEffectContainerSpec UCoreAbility::MakeEffectContainerSpecFromContainer(const FCoreGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel) {
	// First figure out our actor info
	FCoreGameplayEffectContainerSpec ReturnSpec;

	// If we have a target type, run the targeting logic. This is optional, targets can be added later
	if (Container.TargetType.Get()) {
		TArray<FHitResult> HitResults;
		TArray<AActor*> TargetActors;
		const UCoreTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
		ACoreCharacter* TargetingCharacter = Cast<ACoreCharacter>(GetAvatarActorFromActorInfo());
		ACoreCharacterStateBase* TargetingState = Cast<ACoreCharacterStateBase>(GetOwningActorFromActorInfo());
		TargetTypeCDO->GetTargets(TargetingCharacter, TargetingState, EventData, FilterActors, HitResults, TargetActors);
		for (const FHitResult& HitResult : HitResults) {
			FilterActors.Add(HitResult.GetActor());
		}
		for (auto TargetActor : TargetActors) {
			FilterActors.Add(TargetActor);
		}
		ReturnSpec.AddTargets(HitResults, TargetActors);
	}

	const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
	auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();

	// Build GameplayEffectSpecs for each applied effect
	for (const FEffectInfo& EffectInfo : Container.TargetGameplayEffects) {
		FEffectConfigTableRow* EffectRow = (FEffectConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(EffectDataTable, EffectInfo.EffectID);
		int EffectLevel = OverrideGameplayLevel == -1 ? EffectInfo.EffectLevel : OverrideGameplayLevel;
		ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectRow->GameplayEffectClass, EffectLevel));
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

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs) {
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UCoreAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel) {
	FCoreGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}

void UCoreAbility::ClearFilterActors() {
	FilterActors.Empty();
}

bool UCoreAbility::K2_IsActive() const {
	return IsActive();
}

void UCoreAbility::NotifyComboAbility_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, FName const ComboSection) {

}

void UCoreAbility::CallEndAbility() {
	K2_EndAbility();
}

void UCoreAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) {
	K2_InputPressed();
}

void UCoreAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) {
	K2_InputReleased();
}