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

TArray<TSubclassOf<class UGameEventBase>> UPlayerSkillReachLevelConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			USkillLevelUpEvent::StaticClass()
		};
	}
}

void UPlayerSkillReachLevelConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	USkillLevelUpEvent* SkillLevelUpEvent = (USkillLevelUpEvent*)HandleEvent;
	auto EventPlayerState = UGameFrameworkUtils::GetEntityState(SkillLevelUpEvent->Unit);
	if (!EventPlayerState || !EventPlayerState->PlayerComponent) {
		return;
	}
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	if (!ConditionPlayerState || !ConditionPlayerState->PlayerComponent) {
		return;
	}
	UPlayerSkillReachLevelCondition* PlayerSkillReachLevelCondition = (UPlayerSkillReachLevelCondition*)Condition;
	if (SkillLevelUpEvent->Skill == PlayerSkillReachLevelCondition->Skill.SkillID
		&& EventPlayerState->PlayerComponent->RoleID == ConditionPlayerState->PlayerComponent->RoleID) {

		RefreshSatisfy();
	}
}