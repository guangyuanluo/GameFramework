#include "ConditionTriggerSystem.h"
#include "CoreGameInstance.h"
#include "CoreCondition.h"
#include "CoreConditionProgress.h"
#include "GameSystemManager.h"
#include "CorePlayerController.h"
#include "ConditionBlueprintLibrary.h"
#include "GameFramework/PlayerState.h"

/** track the last assigned handle globally */
uint64 UConditionTriggerSystem::LastAssignedSerialNumber = 0;

void UConditionTriggerSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);
}

void UConditionTriggerSystem::Uninitialize() {
	FollowMap.Empty();
	ProgressReserveMap.Empty();

    Super::Uninitialize();
}

void UConditionTriggerSystem::FollowConditions(FConditionTriggerHandler& InOutHandler, const TArray<UCoreConditionProgress*>& Progresses, const FOnAllProgressesSatisfyDelegate& InDelegate) {
	GenerateHandler(InOutHandler);

	FConditionFollowContent FollowContent;
	FollowContent.Progresses = Progresses;
	FollowContent.Handler = InOutHandler;
	FollowContent.Callback = InDelegate;

	FollowMap.Add(InOutHandler, FollowContent);

	for (int Index = 0; Index < Progresses.Num(); ++Index) {
		check(Progresses[Index]->IsInitialized());

		ProgressReserveMap.Add(Progresses[Index], InOutHandler);
		//增加监听
		Progresses[Index]->OnConditionProgressSatisfyUpdate.AddDynamic(this, &UConditionTriggerSystem::OnConditionSatisfyChange);
	}	
	//最后通知，防止在通知中unfollow
	if (IsFollowContentSatisfy(FollowContent)) {
		InDelegate.ExecuteIfBound(InOutHandler);
	}
}

void UConditionTriggerSystem::UnfollowConditions(FConditionTriggerHandler& Handler) {
	auto FindFollowPtr = FollowMap.Find(Handler);
	if (FindFollowPtr) {
		for (int Index = 0; Index < FindFollowPtr->Progresses.Num(); ++Index) {
			//移除监听
			FindFollowPtr->Progresses[Index]->OnConditionProgressSatisfyUpdate.RemoveDynamic(this, &UConditionTriggerSystem::OnConditionSatisfyChange);
			ProgressReserveMap.Remove(FindFollowPtr->Progresses[Index]);
		}
		FollowMap.Remove(Handler);
		Handler.Invalidate();
	}
}

void UConditionTriggerSystem::GenerateHandler(FConditionTriggerHandler& InOutHandler) {
	check(!InOutHandler.IsValid());
	uint64 NewSerialNumber = ++LastAssignedSerialNumber;
	if (!ensureMsgf(NewSerialNumber != FConditionTriggerHandler::MaxSerialNumber, TEXT("Condition listener number has wrapped around!"))) {
		NewSerialNumber = (uint64)1;
	}
	InOutHandler.SetInHandle(NewSerialNumber);
}

void UConditionTriggerSystem::OnConditionSatisfyChange(UCoreConditionProgress* Progress, bool NewSatisfy) {
	auto FindReserveInfoPtr = ProgressReserveMap.Find(Progress);
	if (FindReserveInfoPtr) {
		//刷新满足性
		auto& FollowContent = FollowMap[*FindReserveInfoPtr];
		if (IsFollowContentSatisfy(FollowContent)) {
			FollowContent.Callback.ExecuteIfBound(FollowContent.Handler);
		}
	}
}

bool UConditionTriggerSystem::IsFollowContentSatisfy(const FConditionFollowContent& FollowContent) {
	return UConditionBlueprintLibrary::DoesProgressesSatisfy(FollowContent.Progresses);
}

/****************Wait Condition begin******************/
UWaitCondition::UWaitCondition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		AddToRoot();
	}
}

UWaitCondition* UWaitCondition::StartWaitCondition(class ACorePlayerController* PlayerController, const TArray<UCoreCondition*>& Conditions) {
	UWaitCondition* WaitCondition = NewObject<UWaitCondition>();
	
	auto CharacterState = PlayerController->GetPlayerState<APlayerState>();
	for (int Index = 0; Index < Conditions.Num(); ++Index) {
		auto Condition = Conditions[Index];

		auto ConditionProgress = Condition->GenerateConditionProgress(CharacterState);
		ConditionProgress->Initialize();
		WaitCondition->ConditionProgresses.Add(ConditionProgress);
	}

	auto World = WaitCondition->ConditionProgresses[0]->GetWorld();
	auto GameInstance = Cast<UCoreGameInstance>(World->GetGameInstance());

	FOnAllProgressesSatisfyDelegate Callback;
	Callback.BindUFunction(WaitCondition, TEXT("OnAllProgressesSatisfy"));

	GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>()->FollowConditions(WaitCondition->ConditionTriggerHandler, WaitCondition->ConditionProgresses, Callback);

	return WaitCondition;
}

void UWaitCondition::Cancel() {
	if (ConditionProgresses.Num() == 0) {
		return;
	}

	auto World = ConditionProgresses[0]->GetWorld();
	auto GameInstance = Cast<UCoreGameInstance>(World->GetGameInstance());
	GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>()->UnfollowConditions(ConditionTriggerHandler);

	for (auto ConditionProgress : ConditionProgresses) {
		ConditionProgress->Uninitialize();
	}
	ConditionProgresses.Empty();
}

void UWaitCondition::OnAllProgressesSatisfy() {
	OnSatisfy.Broadcast();

	auto World = ConditionProgresses[0]->GetWorld();
	auto GameInstance = Cast<UCoreGameInstance>(World->GetGameInstance());
	GameInstance->GameSystemManager->GetSystemByClass<UConditionTriggerSystem>()->UnfollowConditions(ConditionTriggerHandler);

	for (auto ConditionProgress : ConditionProgresses) {
		ConditionProgress->Uninitialize();
	}
	ConditionProgresses.Empty();

	RemoveFromRoot();
}

/****************Wait Condition end******************/