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
		auto ConditionProgress = NewObject<UCoreConditionProgress>(nullptr, Condition->ProgressClass);
		ConditionProgress->Condition = Condition;
		ConditionProgress->ProgressOwner = PlayerController;
		WaitCondition->ConditionProgress.Add(ConditionProgress);
	}

	GameInstance->GameSystemManager->GetSystemByClass<UConditionSystem>()->FollowConditions(WaitCondition->ConditionProgress, WaitCondition);

	return WaitCondition;
}

ACoreCharacter* UGameFrameworkUtils::GetClosestCharacterWithinRadius(ACoreCharacter* Source, const FVector& OffsetFromActor, float TraceLength, float Radius, ETraceTypeQuery TraceChannel, ETeamAttitude::Type teamAttitude, bool DrawDebug) {
	auto Result = GetAllCharactersWithinRadius(Source, OffsetFromActor, TraceLength, Radius, TraceChannel, teamAttitude, DrawDebug);
	FVector SourceLocation = Source->GetActorLocation();
	float MinDistance = MAX_flt;
	ACoreCharacter* ClosestActor = nullptr;
	for (int Index = 0; Index < Result.Num(); ++Index) {
		float Distance = FVector::DistSquared(Result[Index]->GetActorLocation(), SourceLocation);
		if (MinDistance > Distance) {
			MinDistance = Distance;
			ClosestActor = Result[Index];
		}
	}
	return ClosestActor;
}

TArray<ACoreCharacter*> UGameFrameworkUtils::GetAllCharactersWithinRadius(ACoreCharacter* Source, const FVector& OffsetFromActor, float TraceLength, float Radius, ETraceTypeQuery TraceChannel, ETeamAttitude::Type TeamAttitude, bool DrawDebug) {
	TArray<ACoreCharacter*> Result;
	TSet<ACoreCharacter*> FilterSet;
	auto SourceAgent = Cast<IGenericTeamAgentInterface>(Source);
	TArray<FHitResult> OutHits;
	FVector Center = Source->GetActorLocation() + OffsetFromActor;
	EDrawDebugTrace::Type DrawDebugTrace = DrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	if (UKismetSystemLibrary::SphereTraceMulti(Source, Center, Center + Source->GetActorForwardVector() * TraceLength, Radius, TraceChannel, false, TArray<AActor*>({ Source }), DrawDebugTrace, OutHits, false)) {
		for (int Index = 0; Index < OutHits.Num(); ++Index) {
			auto& HitResult = OutHits[Index];
			auto Character = Cast<ACoreCharacter>(HitResult.GetActor());
			if (FilterSet.Contains(Character)) {
				continue;
			}
			FilterSet.Add(Character);
			auto Agent = Cast<IGenericTeamAgentInterface>(Character);
			if (Agent && (FGenericTeamId::GetAttitude(SourceAgent->GetGenericTeamId(), Agent->GetGenericTeamId()) == TeamAttitude)) {				
				if (Character) {
					Result.Add(Character);
				}
			}
		}
	}
	return Result;
}

AActor* UGameFrameworkUtils::GetClosestActorWithinRadius(AActor* Source, const FVector& OffsetFromActor, float TraceLength, float Radius, ETraceTypeQuery TraceChannel, bool DrawDebug) {
	auto Result = GetAllActorsWithinRadius(Source, OffsetFromActor, TraceLength, Radius, TraceChannel, DrawDebug);
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

TArray<AActor*> UGameFrameworkUtils::GetAllActorsWithinRadius(AActor* Source, const FVector& OffsetFromActor, float TraceLength, float Radius, ETraceTypeQuery TraceChannel, bool DrawDebug) {
	TArray<AActor*> Result;
	TSet<AActor*> FilterSet;
	TArray<FHitResult> OutHits;
	FVector Center = Source->GetActorLocation() + OffsetFromActor;
	EDrawDebugTrace::Type DrawDebugTrace = DrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	if (UKismetSystemLibrary::SphereTraceMulti(Source, Center, Center + Source->GetActorForwardVector() * TraceLength, Radius, TraceChannel, false, TArray<AActor*>({ Source }), DrawDebugTrace, OutHits, false)) {
		for (int Index = 0; Index < OutHits.Num(); ++Index) {
			auto& HitResult = OutHits[Index];
			AActor* Actor = HitResult.GetActor();
			if (FilterSet.Contains(Actor)) {
				continue;
			}
			FilterSet.Add(Actor);
			Result.Add(Actor);
		}
	}
	return Result;
}

int64 UGameFrameworkUtils::CombineNumber32(int NumberA, int NumberB) {
	int64 Temp = NumberA;
	return (Temp << 32) | NumberB;

}

int64 UGameFrameworkUtils::Conv_StringToInt64(const FString& InString) 	{
	return FCString::Atoi64(*InString);
}

bool UGameFrameworkUtils::IsUE4RunInServer(UObject* WorldContext) {
	auto World = WorldContext->GetWorld();
	auto NetMode = World->GetNetMode();
	return NetMode == ENetMode::NM_DedicatedServer || NetMode == ENetMode::NM_ListenServer;
}

ACoreCharacter* UGameFrameworkUtils::GetCharacterFromGameEffectSpec(const FGameplayEffectSpec& Spec) {
    auto EffectCauser = Spec.GetContext().GetEffectCauser();
    if (EffectCauser) {
        auto CoreCharacter = Cast<ACoreCharacter>(EffectCauser);
        return CoreCharacter;
    }
    return nullptr;
}

class ACoreCharacter* UGameFrameworkUtils::GetCharacterFromComponentOwner(class UActorComponent* ActorComponent) {
    auto Owner = ActorComponent->GetOwner();
    auto PlayerState = Cast<ACoreCharacterStateBase>(Owner);
    if (PlayerState) {
        return Cast<ACoreCharacter>(PlayerState->GetPawn());
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

class AActor* UGameFrameworkUtils::SpawnActor(UObject* WorldContext, TSubclassOf<AActor> ActorClass, FVector const& Location, FRotator const& Rotation, ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride) {
    auto World = WorldContext->GetWorld();
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = SpawnCollisionHandlingOverride;
    return World->SpawnActor<AActor>(ActorClass, Location, Rotation, Params);
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

UCoreGameInstance* UGameFrameworkUtils::GetGameInstance(AActor* Actor) {
	auto World = Actor->GetWorld();
	if (World) {
		return Cast<UCoreGameInstance>(World->GetGameInstance());
	}
	return nullptr;
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
	auto Montage = AnimInstance->GetCurrentActiveMontage();
	if (!IsValid(Montage)) {
		return nullptr;
	}
	auto ActiveMontageInstance = AnimInstance->GetActiveMontageInstance();
	if (!ActiveMontageInstance) {
		return nullptr;
	}
	auto CurrentTrackPosition = ActiveMontageInstance->GetPosition();
	auto FindClass = AnimNotifyStateClass.Get();
	for (int32 Index = 0; Index < Montage->Notifies.Num(); Index++) {
		FAnimNotifyEvent& NotifyEvent = Montage->Notifies[Index];

		if (NotifyEvent.NotifyStateClass && NotifyEvent.NotifyStateClass->GetClass() == FindClass) {
			const float NotifyStartTime = NotifyEvent.GetTriggerTime();
			const float NotifyEndTime = NotifyEvent.GetEndTriggerTime();

			bool bNotifyIsActive = (CurrentTrackPosition > NotifyStartTime) && (CurrentTrackPosition <= NotifyEndTime);
			if (bNotifyIsActive) {
				return NotifyEvent.NotifyStateClass;
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

TArray<uint8> UGameFrameworkUtils::StringToBinary(const FString& Str) {
    TArray<uint8> Bytes;
    Bytes.SetNumUninitialized(Str.Len());
    int32 NumBytes = StringToBytes(Str, Bytes.GetData(), Bytes.Num());

    return Bytes;
}

FString UGameFrameworkUtils::BinaryToString(const TArray<uint8>& Data) {
    if (Data.Num() == 0) {
        return FString();
    }
    else {
        uint8* DataPtr = (uint8*)(void*)&(Data[0]);
        check(DataPtr[0] == Data[0]);
        return BinaryToString(DataPtr, Data.Num());
    }
}

FString UGameFrameworkUtils::BinaryToString(uint8* Data, int num) {
    FString Result = BytesToString(Data, num);
    check(Result.Len() == num);
    return Result;
}

class UGameWorldSubsystem* UGameFrameworkUtils::GetGameWorldSubsystem(UObject* WorldContext, TSubclassOf<class UGameWorldSubsystem> SubsystemClass) {
	auto World = WorldContext->GetWorld();
	if (World) {
		return Cast<UGameWorldSubsystem>(World->GetSubsystemBase(SubsystemClass));
	}
	return nullptr;
}