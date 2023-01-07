// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSkillReachLevelConditionProgress.h"
#include "PlayerSkillReachLevelCondition.h"
#include "SkillLevelUpEvent.h"
#include "GameFrameworkUtils.h"
#include "CoreCharacterStateBase.h"
#include "PlayerComponent.h"
#include "ConfigTableCache.h"
#include "SkillConfigTableRow.h"
#include "SkillSetting.h"
#include "CoreAbilitySystemComponent.h"

void UPlayerSkillReachLevelConditionProgress::PostProgressInitialize_Implementation() {

}

TArray<TSubclassOf<class UGameEventBase>> UPlayerSkillReachLevelConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			USkillLevelUpEvent::StaticClass()
		});
	}
}

bool UPlayerSkillReachLevelConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	USkillLevelUpEvent* SkillLevelUpEvent = (USkillLevelUpEvent*)GameEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(SkillLevelUpEvent->Unit);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return false;
	}
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
		return false;
	}
	UPlayerSkillReachLevelCondition* PlayerSkillReachLevelCondition = (UPlayerSkillReachLevelCondition*)Condition;
	if (SkillLevelUpEvent->Skill == PlayerSkillReachLevelCondition->Skill.SkillID
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {
		return true;
	}
	return false;
}

bool UPlayerSkillReachLevelConditionProgress::IsComplete_Implementation() {
	UPlayerSkillReachLevelCondition* PlayerSkillReachLevelCondition = (UPlayerSkillReachLevelCondition*)Condition;

	const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
	auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
	auto SkillRawRow = UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, PlayerSkillReachLevelCondition->Skill.SkillID);
	if (SkillRawRow) {
		return false;
	}
	FSkillConfigTableRow* SkillRow = (FSkillConfigTableRow*)SkillRawRow;
	auto CharacterState = Cast<ACoreCharacterStateBase>(ProgressOwner);

	TArray<UGameplayAbility*> ActiveAbilities;
	CharacterState->SkillComponent->GetActiveAbilitiesWithClass(SkillRow->GameplayAbilityClass, ActiveAbilities);
	if (ActiveAbilities.Num() == 0) {
		return false;
	}
	return ActiveAbilities[0]->GetAbilityLevel() >= PlayerSkillReachLevelCondition->Skill.SkillLevel;
}

void UPlayerSkillReachLevelConditionProgress::HandleComplete_Implementation() {

}