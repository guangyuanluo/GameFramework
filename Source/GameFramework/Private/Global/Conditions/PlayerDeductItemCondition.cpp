// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerDeductItemCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "OrderCompleteEvent.h"
#include "OrderSystem.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "ItemOrder.h"

UPlayerDeductItemCondition::UPlayerDeductItemCondition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	ProgressClass = UPlayerDeductItemConditionProgress::StaticClass();
}

void UPlayerDeductItemConditionProgress::OnInitialize() {
	Super::OnInitialize();

	auto GameInstance = Cast<UCoreGameInstance>(ProgressOwner->GetWorld()->GetGameInstance());
	if (!GameInstance) {
		return;
	}
	auto OrderSystem = GameInstance->GameSystemManager->GetSystemByClass<UOrderSystem>();
	if (!OrderSystem) {
		return;
	}
	auto ItemOrder = Cast<UItemOrder>(OrderSystem->GenerateNewOrder(UItemOrder::StaticClass(), nullptr));
	if (!ItemOrder) {
		return;
	}
	auto PlayerDeductItemCondition = Cast<UPlayerDeductItemCondition>(Condition);
	ItemOrder->Items = PlayerDeductItemCondition->Items;
	OrderID = ItemOrder->ID;
}

EConditionSatisfy UPlayerDeductItemConditionProgress::GetSatisfy() {
	if (HaveComplete) {
		return EConditionSatisfy::ECS_Satisfy;
	}
	return EConditionSatisfy::ECS_NotSatisfy;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerDeductItemConditionProgress::GetHandleEventTypes() {
	EConditionSatisfy Satisfy = GetSatisfy();
	if (Satisfy == EConditionSatisfy::ECS_Satisfy) {
		return {};
	}
	else {
		return {
			UOrderCompleteEvent::StaticClass(),
		};
	}
}

void UPlayerDeductItemConditionProgress::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UOrderCompleteEvent* OrderCompleteEvent = Cast<UOrderCompleteEvent>(HandleEvent);
	if (OrderCompleteEvent) {
		if (OrderCompleteEvent->OrderID == OrderID) {
			HaveComplete = true;
			RefreshSatisfy();
		}
	}
}

void UPlayerDeductItemConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerDeductItemConditionProgress, HaveComplete, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerDeductItemConditionProgress, OrderID, Params);
}