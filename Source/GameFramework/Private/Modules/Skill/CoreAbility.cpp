#include "CoreAbility.h"
#include "CoreAbilityComboExecutor.h"
#include "CoreCharacter.h"
#include "CoreTargetType.h"
#include "CoreAbilitySystemComponent.h"
#include "ConfigTableCache.h"
#include "EffectConfigTableRow.h"
#include "SkillSetting.h"
#include "CoreCharacterStateBase.h"

UCoreAbility::UCoreAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
	//默认本地预测
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	//默认为每个actor实例
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

FCoreGameplayEffectContainerSpec UCoreAbility::MakeEffectContainerSpecFromContainer(const FCoreGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel) {
	// First figure out our actor info
	FCoreGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	ACoreCharacterStateBase* OwningState = Cast<ACoreCharacterStateBase>(OwningActor);
	UCoreAbilitySystemComponent* OwningASC = Cast<UCoreAbilitySystemComponent>(OwningState->GetAbilitySystemComponent());
	ACoreCharacter* OwningCharacter = Cast<ACoreCharacter>(OwningState->GetPawn());

	if (OwningASC) {
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get()) {
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const UCoreTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default on the ability itself
		if (OverrideGameplayLevel == INDEX_NONE) {
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel(); //OwningASC->GetDefaultAbilityLevel();
		}

		const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
		auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();

		// Build GameplayEffectSpecs for each applied effect
		for (const FEffectInfo& EffectInfo : Container.TargetGameplayEffects) {
			FEffectConfigTableRow* EffectRow = (FEffectConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(EffectDataTable, EffectInfo.EffectID);
			int EffectLevel = OverrideGameplayLevel == -1 ? EffectInfo.EffectLevel : OverrideGameplayLevel;
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectRow->GameplayEffectClass, EffectLevel));
		}
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

bool UCoreAbility::K2_IsActive() const {
	return IsActive();
}

void UCoreAbility::NotifyComboAbility_Implementation(class UCoreAbilitySystemComponent* AbilityComponent, FName const ComboSection) {

}

void UCoreAbility::CallEndAbility() {
	K2_EndAbility();
}