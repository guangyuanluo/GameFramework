#include "CoreAbility.h"
#include "CoreAbilityComboExecutor.h"

UCoreAbility::UCoreAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
	//默认本地预测
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	//默认为每个actor实例
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UCoreAbility::K2_IsActive() const {
	return IsActive();
}

void UCoreAbility::NotifyComboAbility_Implementation(FName const ComboSection) {

}