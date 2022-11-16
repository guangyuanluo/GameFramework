// Fill out your copyright notice in the Description page of Project Settings.

#include "EventSystem.h"
#include "EventHandlerInterface.h"
#include "UE4LogImpl.h"
#include "GameEventBase.h"
#include "Engine/World.h"
#include "AllEvent.h"
#include "CoreGameInstance.h"
#include "CorePlayerController.h"
#include "Serialization/BufferArchive.h"
#include "GameFrameworkUtils.h"

void UEventSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);

	CanTick = true;
}

void UEventSystem::Uninitialize() {
    Super::Uninitialize();

	mEventQueue.Empty();
	{
		for (auto pair : mEventHandlerMap) {
			pair.Value.Empty();
		}
		mEventHandlerMap.Empty();
	}
}

void UEventSystem::PushEvent(UGameEventBase* InPushEvent) {
	UE_LOG(GameCore, VeryVerbose, TEXT("UEventSystem::PushEvent event:[%s]"), *InPushEvent->GetClass()->GetFullName());
    InPushEvent->AddToRoot();
	mEventQueue.Enqueue(InPushEvent);
}

void UEventSystem::PushEventToServer(UGameEventBase* InPushEvent, bool Reliable) {
    auto World = GetWorld();
    auto NetMode = World->GetNetMode();
    if (NetMode == ENetMode::NM_Client) {
        auto PlayerController = Cast< ACorePlayerController>(GameInstance->GetFirstLocalPlayerController());
        if (PlayerController) {
            FBufferArchive ToBinary;
            InPushEvent->Serialize(ToBinary);
            FString SerializeEvent = UGameFrameworkUtils::BinaryToString(ToBinary.GetData(), ToBinary.Num());

			if (Reliable) {
                PlayerController->SendEventToServerReliable(InPushEvent->GetClass()->GetPathName(), SerializeEvent);
			}
			else {
                PlayerController->SendEventToServerUnreliable(InPushEvent->GetClass()->GetPathName(), SerializeEvent);
			}
        }
    }
    else {
        PushEvent(InPushEvent);
    }
}

void UEventSystem::PushEventToClient(class ACorePlayerController* PlayerController, UGameEventBase* InPushEvent) {
    auto World = GetWorld();
    auto NetMode = World->GetNetMode();
    if (NetMode == ENetMode::NM_Client) {
        FBufferArchive ToBinary;
        InPushEvent->Serialize(ToBinary);
        FString SerializeEvent = UGameFrameworkUtils::BinaryToString(ToBinary.GetData(), ToBinary.Num());
        PlayerController->SendEventToClient(InPushEvent->GetClass()->GetPathName(), SerializeEvent);
    }
    else {
        PushEvent(InPushEvent);
    }
}

void UEventSystem::RegistEventHandler(TScriptInterface<IEventHandlerInterface> EventHandler) {
	auto EventTypeList = EventHandler->Execute_GetHandleEventTypes(EventHandler.GetObject());
	{
		for (auto& EventType : EventTypeList) {
			if (!mEventHandlerMap.Contains(EventType)) {
				mEventHandlerMap.Add(EventType, TArray<TScriptInterface<IEventHandlerInterface>>());
			}
			mEventHandlerMap[EventType].Add(EventHandler);
		}
	}
}

void UEventSystem::UnRegistEventHandler(TScriptInterface<IEventHandlerInterface> EventHandler) {
	auto EventTypeList = EventHandler->Execute_GetHandleEventTypes(EventHandler.GetObject());
	{
		for (auto& EventType : EventTypeList) {
			auto FindPtr = mEventHandlerMap.Find(EventType);
			if (FindPtr != nullptr) {
                FindPtr->Remove(EventHandler);
			}
		}
	}
}

void UEventSystem::HandleSendEventToServer(class ACorePlayerController* PlayerController, const FString& EventClass, const FString& SerializeEvent) {
    UClass* FindEventClass = StaticLoadClass(UGameEventBase::StaticClass(), NULL, *EventClass);
    if (FindEventClass) {
        UGameEventBase* GameEvent = NewObject<UGameEventBase>(PlayerController, FindEventClass);
        TArray<uint8> Data = UGameFrameworkUtils::StringToBinary(SerializeEvent);
        FMemoryReader FromBinary = FMemoryReader(Data, true);
        FromBinary.Seek(0);
        GameEvent->Serialize(FromBinary);
        PushEvent(GameEvent);
    }
}

void UEventSystem::HandleSendEventToClient(const FString& EventClass, const FString& SerializeEvent) {
    UClass* FindEventClass = StaticLoadClass(UGameEventBase::StaticClass(), NULL, *EventClass);
    if (FindEventClass) {
        UGameEventBase* GameEvent = NewObject<UGameEventBase>(this, FindEventClass);
        TArray<uint8> Data = UGameFrameworkUtils::StringToBinary(SerializeEvent);
        FMemoryReader FromBinary = FMemoryReader(Data, true);
        FromBinary.Seek(0);
        GameEvent->Serialize(FromBinary);
        PushEvent(GameEvent);
    }
}

void UEventSystem::OnTick_Implementation(float DeltaTime) {
	if (mEventQueue.IsEmpty()) return;
	int HandleCount = 0;
	while (HandleCount < TickHandleEventMax) {
		UGameEventBase* HandleEvent;
		if (!mEventQueue.Dequeue(HandleEvent)) {
			break;
		}
		++HandleCount;
		TArray<TScriptInterface<IEventHandlerInterface>> HandlerList;
		{
			auto FindPtr = mEventHandlerMap.Find(HandleEvent->GetClass());
			if (FindPtr != nullptr) {
                HandlerList = (*FindPtr);
			}
			auto AllPtr = mEventHandlerMap.Find(UAllEvent::StaticClass());
			if (AllPtr != nullptr) {
				for (int Index = 0; Index < AllPtr->Num(); ++Index) {
                    HandlerList.Add((*AllPtr)[Index]);
				}
			}
		}
		if (HandlerList.Num() != 0) {
			for (auto Iter = HandlerList.CreateConstIterator(); Iter; ++Iter) {
				(*Iter)->Execute_OnEvent(Iter->GetObject(), GameInstance, HandleEvent);
			}
		}
		HandleEvent->RemoveFromRoot();
        HandleEvent->ConditionalBeginDestroy();
        HandleEvent = nullptr;
	}
}
