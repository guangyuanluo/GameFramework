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

void UPlayerDeductItemConditionProgress::OnStart_Implementation() {
	Super::OnStart_Implementation();

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

bool UPlayerDeductItemConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

TArray<TSubclassOf<class UGameEventBase>> UPlayerDeductItemConditionProgress::GetHandleEventTypes_Implementation() {
	if (IsComplete()) {
		return {};
	}
	else {
		return {
			UOrderCompleteEvent::StaticClass(),
		};
	}
}

void UPlayerDeductItemConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
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