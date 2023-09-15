// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComboCacheComponent.h"

void USkillComboCacheComponent::SetCurrentSectionName(FName NewSectionName) {
	CurrentSectionName = NewSectionName;
}

FName USkillComboCacheComponent::GetCurrentSectionName() const {
	return CurrentSectionName;
}