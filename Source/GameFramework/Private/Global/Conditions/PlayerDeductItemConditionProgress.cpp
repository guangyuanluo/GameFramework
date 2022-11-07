// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerDeductItemConditionProgress.h"
#include "PlayerDeductItemCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "OrderCompleteEvent.h"
#include "OrderSystem.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "ItemOrder.h"

void UPlayerDeductItemConditionProgress::PostProgressInitialize_Implementation() {
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

TArray<TSubclassOf<class UGameEventBase>> UPlayerDeductItemConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UOrderCompleteEvent::StaticClass(),
		});
	}
}

bool UPlayerDeductItemConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UOrderCompleteEvent* OrderCompleteEvent = Cast<UOrderCompleteEvent>(GameEvent);
	if (OrderCompleteEvent) {
		if (OrderCompleteEvent->OrderID == OrderID) {
			HaveComplete = true;
			return true;
		}
	}
	return false;
}

bool UPlayerDeductItemConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UPlayerDeductItemConditionProgress::HandleComplete_Implementation() {
	
}

void UPlayerDeductItemConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPlayerDeductItemConditionProgress, HaveComplete);
	DOREPLIFETIME(UPlayerDeductItemConditionProgress, OrderID);
}