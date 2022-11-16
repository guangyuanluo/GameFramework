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

TArray<TSubclassOf<class UGameEventBase>> UCloseToNPCConditionProgress::GetCareEventTypes_Implementation() {
	return TArray<TSubclassOf<class UGameEventBase>>({});
}

bool UCloseToNPCConditionProgress::ProgressGameEvent_Implementation(UGameEventBase* GameEvent) {
	return false;
}

bool UCloseToNPCConditionProgress::IsComplete_Implementation() {
	return HaveComplete;
}

void UCloseToNPCConditionProgress::HandleComplete_Implementation() {
	auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
	auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());

	Character->OnCharacterMovementUpdated.RemoveDynamic(this, &UCloseToNPCConditionProgress::OnCharacterMovementUpdate);
}

void UCloseToNPCConditionProgress::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCloseToNPCConditionProgress, HaveComplete);
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
			if (NPCSystem->FindNPCInfo(CloseToNPCCondition->UnitID, NPCInfo) && NPCInfo.NPC) {
				auto ConditionPlayerState = Cast<ACoreCharacterStateBase>(ProgressOwner);
				auto Character = Cast<ACoreCharacter>(ConditionPlayerState->GetPawn());

				auto SizeSquared = (NPCInfo.NPC->GetActorLocation() - Character->GetActorLocation()).SizeSquared();
				auto Distance = NPCInfo.NPC->GetCapsuleComponent()->GetScaledCapsuleRadius() + Character->GetCapsuleComponent()->GetScaledCapsuleRadius() + CloseToNPCCondition->Radius;

				if (Distance * Distance >= SizeSquared) {
					//Âú×ãÌõ¼þ
					HaveComplete = true;
				}
				else {
					HaveComplete = false;
				}
				auto ConditionSystem = GameInstance->GameSystemManager->GetSystemByClass<UConditionSystem>();
				ConditionSystem->NotifyConditionProgressChange(this);
			}
		}
	}
}