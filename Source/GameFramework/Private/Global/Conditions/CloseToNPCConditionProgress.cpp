// Fill out your copyright notice in the Description page of Project Settings.

#include "CloseToNPCConditionProgress.h"
#include "CloseToNPCCondition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "CoreCharacterStateBase.h"
#include "CoreCharacter.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "NPCSystem.h"
#include "ConditionSystem.h"
#include "Components/CapsuleComponent.h"
#include "CorePlayerController.h"

void UCloseToNPCConditionProgress::PostProgressInitialize_Implementation() {
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());
	auto PlayerController = Cast<ACorePlayerController>(Character->GetController());
	
	PlayerController->OnCharacterTemplateUpdated.AddDynamic(this, &UCloseToNPCConditionProgress::OnCharacterTemplateUpdate);
	Character->OnCharacterMovementUpdated.AddDynamic(this, &UCloseToNPCConditionProgress::OnCharacterMovementUpdate);
}

bool UCloseToNPCConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UCloseToNPCConditionProgress::HandleComplete_Implementation() {
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());

	Character->OnCharacterMovementUpdated.RemoveDynamic(this, &UCloseToNPCConditionProgress::OnCharacterMovementUpdate);
}

TArray<TSubclassOf<class UGameEventBase>> UCloseToNPCConditionProgress::GetHandleEventTypes_Implementation() {
	return {};
}

void UCloseToNPCConditionProgress::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {

}

void UCloseToNPCConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params;
	Params.Condition = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UCloseToNPCConditionProgress, HaveComplete, Params);
}

void UCloseToNPCConditionProgress::OnCharacterTemplateUpdate(class ACorePlayerController* PlayerController) {
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());

	Character->OnCharacterMovementUpdated.AddDynamic(this, &UCloseToNPCConditionProgress::OnCharacterMovementUpdate);
}

void UCloseToNPCConditionProgress::OnCharacterMovementUpdate(float DeltaSeconds, FVector OldLocation, FVector OldVelocity) {
	if (OldVelocity.IsNearlyZero()) return;
	auto GameInstance = ProgressOwner->GetWorld()->GetGameInstance<UCoreGameInstance>();
	if (GameInstance) {
		auto NPCSystem = GameInstance->GameSystemManager->GetSystemByClass<UNPCSystem>();
		if (NPCSystem) {
			auto CloseToNPCCondition = Cast<UCloseToNPCCondition>(Condition);
			FNPCInfo NPCInfo;
			if (NPCSystem->FindNPCInfo(CloseToNPCCondition->UnitIDContainer.UnitID, NPCInfo) && NPCInfo.NPC) {
				auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
				auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());

				auto SizeSquared = (NPCInfo.NPC->GetActorLocation() - Character->GetActorLocation()).SizeSquared();
				auto Distance = NPCInfo.NPC->GetCapsuleComponent()->GetScaledCapsuleRadius() + Character->GetCapsuleComponent()->GetScaledCapsuleRadius() + CloseToNPCCondition->Radius;

				if (Distance * Distance >= SizeSquared) {
					//满足条件
					HaveComplete = true;
				}
				else {
					HaveComplete = false;
				}
				RefreshSatisfy();
			}
		}
	}
}