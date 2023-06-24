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

void UPlayerDeductMoneyConditionProgress::OnStart_Implementation() {
	Super::OnStart_Implementation();

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

bool UPlayerDeductMoneyConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerDeductMoneyConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UOrderCompleteEvent::StaticClass(),
		};
	}
}

void UPlayerDeductMoneyConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	UOrderCompleteEvent* OrderCompleteEvent = Cast<UOrderCompleteEvent>(HandleEvent);
	if (OrderCompleteEvent) {
		if (OrderCompleteEvent->OrderID == OrderID) {
			HaveComplete = true;

			RefreshSatisfy();
		}
	}
}

void UPlayerDeductMoneyConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerDeductMoneyConditionProgress, HaveComplete, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UPlayerDeductMoneyConditionProgress, OrderID, Params);
}