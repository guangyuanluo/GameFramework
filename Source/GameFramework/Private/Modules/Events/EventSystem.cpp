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
#include "StringUtils.h"

void UEventSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);

	CanTick = true;
}

void UEventSystem::Uninitialize() {
    Super::Uninitialize();

	EventQueue.Empty();
	{
		for (auto pair : EventHandlerMap) {
			pair.Value.Empty();
		}
		EventHandlerMap.Empty();
	}
}

void UEventSystem::PushEvent(UGameEventBase* InPushEvent) {
	UE_LOG(GameCore, VeryVerbose, TEXT("UEventSystem::PushEvent event:[%s]"), *InPushEvent->GetClass()->GetFullName());
    InPushEvent->AddToRoot();
	EventQueue.Enqueue(InPushEvent);
}

void UEventSystem::PushEventToServer(UGameEventBase* InPushEvent, bool Reliable) {
    auto World = GetWorld();
    auto NetMode = World->GetNetMode();
    if (NetMode == ENetMode::NM_Client) {
        auto PlayerController = Cast<ACorePlayerController>(GameInstance->GetFirstLocalPlayerController());
        if (PlayerController) {
            FBufferArchive ToBinary;
            InPushEvent->Serialize(ToBinary);
            FString SerializeEvent = UStringUtils::BinaryToString(ToBinary.GetData(), ToBinary.Num());

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
        FString SerializeEvent = UStringUtils::BinaryToString(ToBinary.GetData(), ToBinary.Num());
        PlayerController->SendEventToClient(InPushEvent->GetClass()->GetPathName(), SerializeEvent);
    }
    else {
        PushEvent(InPushEvent);
    }
}

void UEventSystem::RegistEventHandler(TScriptInterface<IEventHandlerInterface> EventHandler) {
	auto EventHandlerObj = EventHandler.GetObject();
	if (HandlerEventTypeMap.Contains(EventHandlerObj)) {
		UnRegistEventHandler(EventHandler);
	}

	auto EventTypeList = EventHandler->GetHandleEventTypes();
	{
		HandlerEventTypeMap.Add(EventHandlerObj, EventTypeList);
		for (auto& EventType : EventTypeList) {
			if (!EventHandlerMap.Contains(EventType)) {
				EventHandlerMap.Add(EventType, TArray<UObject*>());
			}
			EventHandlerMap[EventType].Add(EventHandlerObj);
		}
	}
}

void UEventSystem::UnRegistEventHandler(TScriptInterface<IEventHandlerInterface> EventHandler) {
	auto EventHandlerObj = EventHandler.GetObject();
	auto FindEventTypeListPtr = HandlerEventTypeMap.Find(EventHandlerObj);
	if (FindEventTypeListPtr) {
		for (auto& EventType : *FindEventTypeListPtr) {
			auto FindPtr = EventHandlerMap.Find(EventType);
			if (FindPtr != nullptr) {
				FindPtr->Remove(EventHandlerObj);
			}
		}
		HandlerEventTypeMap.Remove(EventHandlerObj);
	}
}

void UEventSystem::RefreshEventHandler(TScriptInterface<IEventHandlerInterface> EventHandler) {
	auto EventHandlerObj = EventHandler.GetObject();
	if (HandlerEventTypeMap.Contains(EventHandlerObj)) {
		UnRegistEventHandler(EventHandler);
		RegistEventHandler(EventHandler);
	}
}

void UEventSystem::HandleSendEventToServer(class ACorePlayerController* PlayerController, const FString& EventClass, const FString& SerializeEvent) {
    UClass* FindEventClass = StaticLoadClass(UGameEventBase::StaticClass(), NULL, *EventClass);
    if (FindEventClass) {
        UGameEventBase* GameEvent = NewObject<UGameEventBase>(PlayerController, FindEventClass);
        TArray<uint8> Data = UStringUtils::StringToBinary(SerializeEvent);
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
        TArray<uint8> Data = UStringUtils::StringToBinary(SerializeEvent);
        FMemoryReader FromBinary = FMemoryReader(Data, true);
        FromBinary.Seek(0);
        GameEvent->Serialize(FromBinary);
        PushEvent(GameEvent);
    }
}

void UEventSystem::OnTick_Implementation(float DeltaTime) {
	if (EventQueue.IsEmpty()) return;
	int HandleCount = 0;
	while (HandleCount < TickHandleEventMax) {
		UGameEventBase* HandleEvent;
		if (!EventQueue.Dequeue(HandleEvent)) {
			break;
		}
		++HandleCount;
		TArray<UObject*> HandlerList;
		{
			auto FindPtr = EventHandlerMap.Find(HandleEvent->GetClass());
			if (FindPtr != nullptr) {
                HandlerList = (*FindPtr);
			}
			auto AllPtr = EventHandlerMap.Find(UAllEvent::StaticClass());
			if (AllPtr != nullptr) {
				for (int Index = 0; Index < AllPtr->Num(); ++Index) {
                    HandlerList.Add((*AllPtr)[Index]);
				}
			}
		}
		if (HandlerList.Num() != 0) {
			for (auto Iter = HandlerList.CreateConstIterator(); Iter; ++Iter) {
				if (auto EventHandlerInterface = Cast<IEventHandlerInterface>(*Iter)) {
					EventHandlerInterface->OnEvent(GameInstance, HandleEvent);
				}
			}
		}
		HandleEvent->RemoveFromRoot();
        HandleEvent->ConditionalBeginDestroy();
        HandleEvent = nullptr;
	}
}
