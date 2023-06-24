#include "ConditionSystem.h"
#include "CoreGameInstance.h"
#include "CoreCondition.h"
#include "CoreConditionProgress.h"
#include "BooleanAlgebraUtil.h"

void UConditionSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);
}

void UConditionSystem::Uninitialize() {
	FollowMap.Empty();
	ProgressReserveMap.Empty();

    Super::Uninitialize();
}

void UConditionSystem::FollowConditions(const TArray<UCoreConditionProgress*>& Progresses, TScriptInterface<ICoreConditionObserver> Observer) {
	auto ObserverObj = Observer.GetObject();
	check(!FollowMap.Contains(ObserverObj));

	FConditionFollowContent FollowContent;
	FollowContent.Progresses = Progresses;
	FollowContent.Observer = Observer;

	FollowMap.Add(ObserverObj, FollowContent);

	for (int Index = 0; Index < Progresses.Num(); ++Index) {
		bool bComplete = Progresses[Index]->IsComplete();

		FProgressReserveInfo ProgressReserveInfo;;
		ProgressReserveInfo.ProgressFollowContent = &FollowMap[ObserverObj];
		ProgressReserveInfo.LastComplete = bComplete;
		ProgressReserveMap.Add(Progresses[Index], ProgressReserveInfo);

		Progresses[Index]->OnStart();

		//增加监听
		Progresses[Index]->OnConditionProgressSatisfyUpdate.AddDynamic(this, &UConditionSystem::OnConditionSatisfyChange);
	}
	//最后通知，防止在通知中unfollow
	if (IsFollowContentSatisfy(FollowContent)) {
		ICoreConditionObserver::Execute_OnSatisfyConditions(ObserverObj, Progresses);
	}
}

void UConditionSystem::UnfollowConditions(TScriptInterface<ICoreConditionObserver> Observer) {
	auto ObserverObj = Observer.GetObject();
	auto FindFollowPtr = FollowMap.Find(ObserverObj);
	if (FindFollowPtr) {
		for (int Index = 0; Index < FindFollowPtr->Progresses.Num(); ++Index) {
			FindFollowPtr->Progresses[Index]->OnEnd();
			//移除监听
			FindFollowPtr->Progresses[Index]->OnConditionProgressSatisfyUpdate.RemoveDynamic(this, &UConditionSystem::OnConditionSatisfyChange);
			ProgressReserveMap.Remove(FindFollowPtr->Progresses[Index]);
		}
		FollowMap.Remove(ObserverObj);
	}
}

TScriptInterface<ICoreConditionObserver> UConditionSystem::GetObserverFromProgress(UCoreConditionProgress* InProgress) {
	auto ProgressReserveInfoPtr = ProgressReserveMap.Find(InProgress);
	if (ProgressReserveInfoPtr) {
		return ProgressReserveInfoPtr->ProgressFollowContent->Observer;
	}
	return nullptr;
}

void UConditionSystem::OnConditionSatisfyChange(UCoreConditionProgress* Progress, bool NewSatisfy) {
	auto FindReserveInfoPtr = ProgressReserveMap.Find(Progress);
	if (FindReserveInfoPtr && FindReserveInfoPtr->LastComplete != NewSatisfy) {
		//刷新满足性
		FindReserveInfoPtr->LastComplete = NewSatisfy;
		if (IsFollowContentSatisfy(*FindReserveInfoPtr->ProgressFollowContent)) {
			auto Observer = FindReserveInfoPtr->ProgressFollowContent->Observer;
			ICoreConditionObserver::Execute_OnSatisfyConditions(Observer.GetObject(), FindReserveInfoPtr->ProgressFollowContent->Progresses);
		}
	}
}

bool UConditionSystem::IsFollowContentSatisfy(const FConditionFollowContent& FollowContent) {
	bool HaveComplete = true;

	TArray<BooleanAlgebraEnum> LoopGroupRelations;
	for (int ProgressIndex = 0; ProgressIndex < FollowContent.Progresses.Num(); ++ProgressIndex) {
		LoopGroupRelations.Add(FollowContent.Progresses[ProgressIndex]->Condition->Relation);
	}
	FBooleanAlgebraNodeInfo GroupExecuteRoot = UBooleanAlgebraUtil::RelationsGenerate(LoopGroupRelations);
	TFunction<bool(int)> CheckFunction = [this, &FollowContent](int ProgressIndex) {
		auto Progress = FollowContent.Progresses[ProgressIndex];
		auto Condition = Progress->Condition;

		bool bLastSatisfy = ProgressReserveMap[Progress].LastComplete;
		return bLastSatisfy != Condition->bNot;
	};

	HaveComplete = UBooleanAlgebraUtil::ExecuteConditionRelationTree(GroupExecuteRoot, CheckFunction);

	return HaveComplete;
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

UWaitCondition* UWaitCondition::StartWaitCondition(UWaitCondition* WaitCondition) {
	return WaitCondition;
}

void UWaitCondition::Cancel() {
	Unfollow();
}

void UWaitCondition::OnSatisfyConditions_Implementation(const TArray<UCoreConditionProgress*>& Progresses) {
	OnSatisfy.Broadcast();
	Unfollow();
	RemoveFromRoot();
}

void UWaitCondition::OnProgressRefresh_Implementation(UCoreConditionProgress* ChangeProgress) {

}

/****************Wait Condition end******************/