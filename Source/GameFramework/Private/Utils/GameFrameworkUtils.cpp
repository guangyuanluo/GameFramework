// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFrameworkUtils.h"
#include "CoreGameInstance.h"
#include "CoreCharacter.h"
#include "ConditionSystem.h"
#include "CoreCondition.h"
#include "Engine/World.h"
#include "QuestSystem.h"
#include "CorePlayerController.h"
#include "CoreGameMode.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CoreConditionProgress.h"
#include "GameSystemManager.h"
#include "CoreCharacterStateBase.h"
#include "PlayerComponent.h"
#include "GameWorldSubsystem.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

UWaitCondition* UGameFrameworkUtils::WaitCondition(class ACorePlayerController* PlayerController, const TArray<UCoreCondition*>& Conditions) {
    auto GameInstance = PlayerController->GetWorld()->GetGameInstance<UCoreGameInstance>();

	UWaitCondition* WaitCondition = NewObject<UWaitCondition>();	
	for (int Index = 0; Index < Conditions.Num(); ++Index) {
		auto Condition = Conditions[Index];
		auto ConditionProgress = Condition->GenerateConditionProgress(PlayerController);
		WaitCondition->ConditionProgress.Add(ConditionProgress);
	}

	GameInstance->GameSystemManager->GetSystemByClass<UConditionSystem>()->FollowConditions(WaitCondition->ConditionProgress, WaitCondition);

	return WaitCondition;
}

AActor* UGameFrameworkUtils::GetClosestActorWithinRadius(AActor* Source, const TArray<AActor*>& IgnoreActors, const FVector& OffsetFromActor, float TraceLength, float Radius, ETraceTypeQuery TraceChannel, ETeamAttitude::Type TeamAttitude, TSubclassOf<AActor> LimitActorClass, bool DrawDebug) {
	auto Result = GetAllActorsWithinRadius(Source, IgnoreActors, OffsetFromActor, TraceLength, Radius, TraceChannel, TeamAttitude, LimitActorClass, false, DrawDebug);
	FVector SourceLocation = Source->GetActorLocation();
	float MinDistance = MAX_flt;
	AActor* ClosestActor = nullptr;
	for (int Index = 0; Index < Result.Num(); ++Index) {
		float Distance = FVector::DistSquared(Result[Index]->GetActorLocation(), SourceLocation);
		if (MinDistance > Distance) {
			MinDistance = Distance;
			ClosestActor = Result[Index];
		}
	}
	return ClosestActor;
}

TArray<AActor*> UGameFrameworkUtils::GetAllActorsWithinRadius(AActor* Source, const TArray<AActor*>& IgnoreActors, const FVector& OffsetFromActor, float TraceLength, float Radius, ETraceTypeQuery TraceChannel, ETeamAttitude::Type TeamAttitude, TSubclassOf<AActor> LimitActorClass, bool SortByDistance, bool DrawDebug) {
	TArray<AActor*> Result;
	TSet<AActor*> FilterSet;
	TArray<FHitResult> OutHits;
	auto SourceAgent = Cast<IGenericTeamAgentInterface>(Source);
	FTransform SourceTransform = Source->GetActorTransform();
	FTransform OffsetTransform;
	OffsetTransform.SetLocation(OffsetFromActor);
	FTransform FinalTransform = OffsetTransform * SourceTransform;
	FVector Center = FinalTransform.GetLocation();
	EDrawDebugTrace::Type DrawDebugTrace = DrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	//保证end和start不一样，不然会导致impactpoint不对
	TraceLength = FMath::IsNearlyZero(TraceLength) ? 0.001f : TraceLength;
	if (UKismetSystemLibrary::SphereTraceMulti(Source, Center, Center + Source->GetActorForwardVector() * TraceLength, Radius, TraceChannel, false, IgnoreActors, DrawDebugTrace, OutHits, false)) {
		for (int Index = 0; Index < OutHits.Num(); ++Index) {
			auto& HitResult = OutHits[Index];
			AActor* Actor = HitResult.GetActor();
			if (!Actor) {
				continue;
			}
			if (FilterSet.Contains(Actor)) {
				continue;
			}
			FilterSet.Add(Actor);
            if (LimitActorClass && !Actor->IsA(LimitActorClass)) {
                continue;
            }
            auto Agent = Cast<IGenericTeamAgentInterface>(Actor);
            if (!SourceAgent || (Agent && (FGenericTeamId::GetAttitude(SourceAgent->GetGenericTeamId(), Agent->GetGenericTeamId()) == TeamAttitude))) {
				Result.Add(Actor);
			}
		}
	}
	return Result;
}

int64 UGameFrameworkUtils::CombineNumber32(int NumberA, int NumberB) {
	int64 Temp = NumberA;
	return (Temp << 32) | NumberB;

}

bool UGameFrameworkUtils::IsUE4RunInServer(UObject* WorldContext) {
	auto World = WorldContext->GetWorld();
	auto NetMode = World->GetNetMode();
	return NetMode == ENetMode::NM_DedicatedServer || NetMode == ENetMode::NM_ListenServer;
}

bool UGameFrameworkUtils::IsUE4RunInEditor() {
	return GEditor != nullptr;
}

class ACoreCharacter* UGameFrameworkUtils::GetCharacterFromComponentOwner(class UActorComponent* ActorComponent) {
    auto Owner = ActorComponent->GetOwner();
    auto PlayerState = Cast<ACoreCharacterStateBase>(Owner);
    if (PlayerState) {
        auto Pawn = Cast<ACoreCharacter>(PlayerState->GetPawn());
		if (Pawn) {
			return Pawn;
		}
    }

    ACoreCharacter* Character = Cast<ACoreCharacter>(Owner);
    if (Character) {
        return Character;
    }
    ACorePlayerController* PlayerController = Cast<ACorePlayerController>(Owner);
    if (PlayerController) {
        Character = Cast<ACoreCharacter>(PlayerController->GetCharacter());
        if (Character) {
            return Character;
        }
    }
    return nullptr;
}

class ACoreCharacterStateBase* UGameFrameworkUtils::GetEntityState(TScriptInterface<IGameEntity> Entity) {
    auto EntityObject = Entity.GetObject();
    if (EntityObject) {
		auto PlayerState = Cast<ACoreCharacterStateBase>(EntityObject);
		if (PlayerState) {
			return PlayerState;
		}
        auto Character = Cast<ACharacter>(EntityObject);
        if (Character) {
            return Cast<ACoreCharacterStateBase>(Character->GetPlayerState());
        }
    }
    return nullptr;
}

class ACoreCharacterStateBase* UGameFrameworkUtils::GetActorState(class AActor* Actor) {
    auto Character = Cast<ACharacter>(Actor);
    if (Character) {
        return Cast<ACoreCharacterStateBase>(Character->GetPlayerState());
    }
    return nullptr;
}

UObject* UGameFrameworkUtils::GetClassDefaultObject(UClass* InClass) {
	if (InClass) {
		return InClass->GetDefaultObject();
	}
	else {
		return nullptr;
	}
}

FString UGameFrameworkUtils::GetRoleID(class AActor* Actor) {
	auto State = Cast<ACoreCharacterStateBase>(Actor);
	if (!State) {
		State = GetActorState(Actor);
	}
	if (State) {
		auto PlayerComponent = Cast<UPlayerComponent>(State->GetComponentByClass(UPlayerComponent::StaticClass()));
		if (PlayerComponent) {
			return PlayerComponent->RoleID;
		}
	}
	return "";
}

ETeamAttitude::Type UGameFrameworkUtils::GetTeamAttribute(AActor* ActorA, AActor* ActorB) {
	return FGenericTeamId::GetAttitude(ActorA, ActorB);
}

ETeamAttitude::Type UGameFrameworkUtils::GetActorTeamAttributeWithTeam(AActor* ActorA, TeamIdDefines Team) {
	const IGenericTeamAgentInterface* TeamAgentA = Cast<const IGenericTeamAgentInterface>(ActorA);

	return TeamAgentA == NULL ? ETeamAttitude::Neutral : FGenericTeamId::GetAttitude(TeamAgentA->GetGenericTeamId(), (FGenericTeamId)(uint8)Team);
}

FString UGameFrameworkUtils::GetEntityID(class AActor* Actor) {
	auto State = Cast<ACoreCharacterStateBase>(Actor);
	if (!State) {
		State = GetActorState(Actor);
	}
	if (State) {
		auto GameEntity = Cast<IGameEntity>(State->GetPawn());
		if (GameEntity) {
			return GameEntity->GetEntityID();
		}
	}
	return "";
}

bool UGameFrameworkUtils::IsTemplateObject(UObject* InObject) {
	return InObject->IsTemplate();
}

UAnimNotifyState* UGameFrameworkUtils::GetAnyActiveAnimNotifyStateByClass(USkeletalMeshComponent* MeshComp, TSubclassOf<UAnimNotifyState> AnimNotifyStateClass) {
	if (!IsValid(MeshComp)) {
		return nullptr;
	}
	auto AnimInstance = MeshComp->GetAnimInstance();
	if (!IsValid(AnimInstance)) {
		return nullptr;
	}

	for (auto MontageInstance : AnimInstance->MontageInstances) {
		if (!MontageInstance || !MontageInstance->Montage) {
			continue;
		}

		auto CurrentTrackPosition = MontageInstance->GetPosition();
		auto FindClass = AnimNotifyStateClass.Get();
		for (int32 Index = 0; Index < MontageInstance->Montage->Notifies.Num(); ++Index) {
			FAnimNotifyEvent& NotifyEvent = MontageInstance->Montage->Notifies[Index];

			if (NotifyEvent.NotifyStateClass && NotifyEvent.NotifyStateClass->GetClass()->IsChildOf(FindClass)) {
				const float NotifyStartTime = NotifyEvent.GetTriggerTime();
				const float NotifyEndTime = NotifyEvent.GetEndTriggerTime();

				bool bNotifyIsActive = (CurrentTrackPosition > NotifyStartTime) && (CurrentTrackPosition <= NotifyEndTime);
				if (bNotifyIsActive) {
					return NotifyEvent.NotifyStateClass;
				}
			}
		}
	}
	
	return nullptr;
}

bool UGameFrameworkUtils::IsMontageValidSection(class UAnimMontage* AnimMontage, FName const& SectionName) {
	if (!IsValid(AnimMontage)) {
		return false;
	}
	const int32 SectionID = AnimMontage->GetSectionIndex(SectionName);

	return AnimMontage->IsValidSectionIndex(SectionID);
}

FName UGameFrameworkUtils::GetMontageNextSection(class UAnimInstance* AnimInstance) {
	if (!IsValid(AnimInstance)) {
		return FName();
	}
	auto MontageInstance = AnimInstance->GetActiveMontageInstance();
	if (!MontageInstance) {
		return FName();
	}
	auto CurrentSecionID = MontageInstance->Montage->GetSectionIndex(MontageInstance->GetCurrentSection());
	auto NextSectionID = MontageInstance->GetNextSectionID(CurrentSecionID);
	return MontageInstance->GetSectionNameFromID(NextSectionID);
}

class UGameWorldSubsystem* UGameFrameworkUtils::GetGameWorldSubsystem(UObject* WorldContext, TSubclassOf<class UGameWorldSubsystem> SubsystemClass) {
    auto World = WorldContext->GetWorld();
    if (World) {
        return Cast<UGameWorldSubsystem>(World->GetSubsystemBase(SubsystemClass));
    }
    return nullptr;
}