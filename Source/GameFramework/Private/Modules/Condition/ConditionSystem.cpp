#include "ConditionSystem.h"
#include "CoreGameInstance.h"
#include "EventSystem.h"
#include "EventHandlerInterface.h"
#include "CoreCondition.h"
#include "DataTableSeedConfigTableHelper.h"
#include "DataTableSeedConfigTableRow.h"
#include "AllEvent.h"
#include "CoreConditionProgress.h"
#include "GameSystemManager.h"
#include "GameEntityManager.h"

void UConditionSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);

	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->RegistEventHandler(this);
}

void UConditionSystem::Uninitialize() {
	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->UnRegistEventHandler(this);
	FollowMap.Empty();
	ProgressReserveMap.Empty();
	ProgressEventMap.Empty();

    Super::Uninitialize();
}

void UConditionSystem::FollowConditions(const TArray<UCoreConditionProgress*>& Progresses, TScriptInterface<ICoreConditionObserver> Observer) {
	auto ObserverObj = Observer.GetObject();
	check(!FollowMap.Contains(ObserverObj));

	UFollowContent* FollowContent = NewObject<UFollowContent>();
	FollowContent->Progresses = Progresses;
	FollowContent->Observer = Observer;

	FollowMap.Add(ObserverObj, FollowContent);

	bool HaveComplete = true;
	for (int Index = 0; Index < Progresses.Num(); ++Index) {
		TArray<TSubclassOf<class UGameEventBase>> CareEventTypes = Progresses[Index]->GetCareEventTypes();
		TArray<UClass*> EventClasses;
		for (int EventTypeIndex = 0; EventTypeIndex < CareEventTypes.Num(); ++EventTypeIndex) {
			EventClasses.Add(CareEventTypes[EventTypeIndex]);
		}

		bool bComplete = Progresses[Index]->IsComplete();

		UProgressReserveInfo* ProgressReserveInfo = NewObject<UProgressReserveInfo>();
		ProgressReserveInfo->ProgressFollowContent = FollowContent;
		ProgressReserveInfo->CareEventTypes = EventClasses;
		ProgressReserveInfo->LastComplete = bComplete;
		ProgressReserveMap.Add(Progresses[Index], ProgressReserveInfo);

		if (!bComplete) {
			HaveComplete = false;
		}

		for (int EventIndex = 0; EventIndex < CareEventTypes.Num(); ++EventIndex) {
			auto& ProgressEventMapValue = ProgressEventMap.FindOrAdd(CareEventTypes[EventIndex]);
			ProgressEventMapValue.Values.Add(Progresses[Index]);
		}
	}

	//最后通知，防止在通知中unfollow
	if (HaveComplete) {
		Observer->Execute_OnSatisfyConditions(ObserverObj, Progresses);
	}
}

void UConditionSystem::UnfollowConditions(TScriptInterface<ICoreConditionObserver> Observer) {
	auto ObserverObj = Observer.GetObject();
	if (FollowMap.Contains(ObserverObj)) {
		UFollowContent* FollowContent = FollowMap[ObserverObj];
		FollowMap.Remove(ObserverObj);
		for (int Index = 0; Index < FollowContent->Progresses.Num(); ++Index) {
			TArray<UClass*>& CareEventTypes = ProgressReserveMap[FollowContent->Progresses[Index]]->CareEventTypes;
			for (int EventIndex = 0; EventIndex < CareEventTypes.Num(); ++EventIndex) {
				auto FindProgressesPtr = ProgressEventMap.Find(CareEventTypes[EventIndex]);
				if (FindProgressesPtr != nullptr) {
					FindProgressesPtr->Values.Remove(FollowContent->Progresses[Index]);
					if (FindProgressesPtr->Values.Num() == 0) {
						ProgressEventMap.Remove(CareEventTypes[EventIndex]);
					}
				}
			}
			ProgressReserveMap.Remove(FollowContent->Progresses[Index]);
		}
	}
}

TScriptInterface<ICoreConditionObserver> UConditionSystem::GetObserverFromProgress(UCoreConditionProgress* InProgress) {
	auto ProgressReserveInfoPtr = ProgressReserveMap.Find(InProgress);
	if (ProgressReserveInfoPtr) {
		return (*ProgressReserveInfoPtr)->ProgressFollowContent->Observer;
	}
	return nullptr;
}

TArray<UClass*> UConditionSystem::GetHandleEventTypes_Implementation() {
	return TArray<UClass*>({ UAllEvent::StaticClass() });
}


void UConditionSystem::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	auto Progresses = ProgressEventMap.Find(HandleEvent->GetClass());
	if (Progresses != nullptr) {
		TArray<UCoreConditionProgress*> ChangeCompleteProgresses;
		TArray<UFollowContent*> SatisfyObservers;
		{
			TSet<UObject*> SatisfyObserverSet;
			//通知进度变化
			for (int Index = 0; Index < Progresses->Values.Num(); ++Index) {
				auto Progress = Progresses->Values[Index];
				bool bHandleEvent = Progress->ProgressGameEvent(HandleEvent);
				if (bHandleEvent) {
					UProgressReserveInfo* ProgressReserveInfo = ProgressReserveMap[Progress];
					auto& FollowContent = ProgressReserveInfo->ProgressFollowContent;
					TScriptInterface<ICoreConditionObserver> Observer = FollowContent->Observer;
					Observer->Execute_OnProgressRefresh(Observer.GetObject(), Progress);

					bool bComplete = Progress->IsComplete();
					if (bComplete != ProgressReserveInfo->LastComplete) {
						ChangeCompleteProgresses.Add(Progress);
						ProgressReserveInfo->LastComplete = bComplete;

						//满足条件，可以检查这个观察者所有的进度，看是否全部完成
						if (bComplete && !SatisfyObserverSet.Contains(Observer.GetObject())) {

							bool bSatisfy = true;
							for (auto OtherProgress : FollowContent->Progresses) {
								if (Progress != OtherProgress) {
									if (!OtherProgress->IsComplete()) {
										bSatisfy = false;
										break;
									}
								}
							}
							if (bSatisfy) {
								SatisfyObserverSet.Add(Observer->_getUObject());
								SatisfyObservers.Add(FollowContent);
							}
						}
					}
				}
			}
		}
		{
			//通知全部满足条件的观察者
			for (auto FollowContent : SatisfyObservers) {
				auto Observer = FollowContent->Observer;
				FollowContent->Observer->Execute_OnSatisfyConditions(Observer.GetObject(), FollowContent->Progresses);
			}
		}
		{
			//最后变化的进度，处理监听条件
			for (auto Progress : ChangeCompleteProgresses) {
				//这里有可能在OnSatisfyConditions被unfollow了，要加非法判断
				UProgressReserveInfo** ProgressReserveInfo = ProgressReserveMap.Find(Progress);
				if (ProgressReserveInfo) {
					//这里说明还在，EventClassType这里对比，如果不同就要重新赋值
					const auto& OldCareEventTypes = (*ProgressReserveInfo)->CareEventTypes;
					auto NewCareEventTypes = Progress->GetCareEventTypes();
					bool bCareEventTypeChange = false;
					if (OldCareEventTypes.Num() != NewCareEventTypes.Num()) {
						bCareEventTypeChange = true;
					}
					else {
						for (int EventTypeIndex = 0; EventTypeIndex < OldCareEventTypes.Num(); ++EventTypeIndex) {
							if (OldCareEventTypes[EventTypeIndex] != NewCareEventTypes[EventTypeIndex]) {
								bCareEventTypeChange = true;
								break;
							}
						}
					}
					if (bCareEventTypeChange) {
						//移除老的
						for (auto EventTypeIndex = 0; EventTypeIndex < OldCareEventTypes.Num(); ++EventTypeIndex) {
							auto CareEventType = (*ProgressReserveInfo)->CareEventTypes[EventTypeIndex];
							auto FindEventTypeInfoPtr = ProgressEventMap.Find(CareEventType);
							if (FindEventTypeInfoPtr) {
								FindEventTypeInfoPtr->Values.Remove(Progress);
								if (FindEventTypeInfoPtr->Values.Num() == 0) {
									ProgressEventMap.Remove(CareEventType);
								}
							}
						}
						(*ProgressReserveInfo)->CareEventTypes.Empty();
						for (auto EventTypeIndex = 0; EventTypeIndex < NewCareEventTypes.Num(); ++EventTypeIndex) {
							auto CareEventType = NewCareEventTypes[EventTypeIndex];
							(*ProgressReserveInfo)->CareEventTypes.Add(CareEventType);
							auto& FindEventTypeInfo = ProgressEventMap.FindOrAdd(CareEventType);
							FindEventTypeInfo.Values.Add(Progress);
						}
					}
				}
			}
		}
	}
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