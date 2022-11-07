// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerDeductMoneyConditionProgress.h"
#include "PlayerDeductMoneyCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "OrderCompleteEvent.h"
#include "OrderSystem.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "MoneyOrder.h"

void UPlayerDeductMoneyConditionProgress::PostProgressInitialize_Implementation() {
	auto GameInstance = Cast<UCoreGameInstance>(ProgressOwner->GetWorld()->GetGameInstance());
	if (!GameInstance) {
		return;
	}
	auto OrderSystem = GameInstance->GameSystemManager->GetSystemByClass<UOrderSystem>();
	if (!OrderSystem) {
		return;
	}
	auto MoneyOrder = Cast<UMoneyOrder>(OrderSystem->GenerateNewOrder(UMoneyOrder::StaticClass(), nullptr));
	if (!MoneyOrder) {
		return;
	}
	auto PlayerDeductMoneyCondition = Cast<UPlayerDeductMoneyCondition>(Condition);
	MoneyOrder->Money = PlayerDeductMoneyCondition->Money;

	OrderID = MoneyOrder->ID;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerDeductMoneyConditionProgress::GetCareEventTypes_Implementation() {
	if (IsComplete()) {
		return TArray<TSubclassOf<class UGameEventBase>>();
	}
	else {
		return TArray<TSubclassOf<class UGameEventBase>>({
			UOrderCompleteEvent::StaticClass(),
		});
	}
}

bool UPlayerDeductMoneyConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	UOrderCompleteEvent* OrderCompleteEvent = Cast<UOrderCompleteEvent>(GameEvent);
	if (OrderCompleteEvent) {
		if (OrderCompleteEvent->OrderID == OrderID) {
			HaveComplete = true;
			return true;
		}
	}
	return false;
}

bool UPlayerDeductMoneyConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UPlayerDeductMoneyConditionProgress::HandleComplete_Implementation() {

}

void UPlayerDeductMoneyConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerDeductMoneyConditionProgress, HaveComplete);
	DOREPLIFETIME(UPlayerDeductMoneyConditionProgress, OrderID);
}