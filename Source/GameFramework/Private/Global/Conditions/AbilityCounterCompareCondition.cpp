// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilityCounterCompareCondition.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "AbilityCounterSubsystem.h"

UAbilityCounterCompareCondition::UAbilityCounterCompareCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UAbilityCounterCompareConditionProgress::StaticClass();
}

void UAbilityCounterCompareConditionProgress::OnInitialize() {
	Super::OnInitialize();

	auto ThisCondition = Cast<UAbilityCounterCompareCondition>(Condition);
	if (ThisCondition->IsGlobal) {
		CounterName = ThisCondition->CounterName;
	}
	else {
		CounterName = FString::Format(TEXT("{0}_{1}"), { *ThisCondition->CounterName, *ProgressOwner->GetName()});
	}

	auto AbilityCounterSubsystem = Cast<UAbilityCounterSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UAbilityCounterSubsystem::StaticClass()));
	AbilityCounterSubsystem->OnAbilityCounterNumChange.AddDynamic(this, &UAbilityCounterCompareConditionProgress::OnCounterChange);
}

void UAbilityCounterCompareConditionProgress::OnUninitialize() {
	Super::OnUninitialize();

	auto AbilityCounterSubsystem = Cast<UAbilityCounterSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UAbilityCounterSubsystem::StaticClass()));
	AbilityCounterSubsystem->OnAbilityCounterNumChange.RemoveDynamic(this, &UAbilityCounterCompareConditionProgress::OnCounterChange);
}

bool UAbilityCounterCompareConditionProgress::IsComplete(bool& IsValid) {
	IsValid = true;
	auto AbilityCounterSubsystem = Cast<UAbilityCounterSubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UAbilityCounterSubsystem::StaticClass()));
	auto ThisCondition = Cast<UAbilityCounterCompareCondition>(Condition);
	return UComparisonOperatorUtil::CheckComparePass(AbilityCounterSubsystem->GetCounterNum(CounterName), ThisCondition->Value, ThisCondition->Operator);
}

void UAbilityCounterCompareConditionProgress::OnCounterChange(const FString& InCounterName, int InCounterNum) {
	if (CounterName != InCounterName) {
		return;
	}
	RefreshSatisfy();
}