// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Containers/Queue.h"
#include "Base/ECS/SystemBase.h"
#include "EventSystem.generated.h"

class UCoreGameInstance;
class UGameEventBase;
class IEventHandlerInterface;

/**
 * 事件系统
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UEventSystem : public USystemBase, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(UCoreGameInstance* InGameInstance) override;
	virtual void Uninitialize() override;

	UFUNCTION(BlueprintCallable, Category = "EventSystem")
	void PushEvent(UGameEventBase* InPushEvent);

    UFUNCTION(BlueprintCallable, Category = "EventSystem")
    void PushEventToServer(UGameEventBase* InPushEvent, bool Reliable);

    UFUNCTION(BlueprintCallable, Category = "EventSystem")
    void PushEventToClient(class ACorePlayerController* PlayerController, UGameEventBase* InPushEvent);
	

	/*
	* 注册/注销eventhandler
	*/
	UFUNCTION(BlueprintCallable, Category = "EventSystem")
	void RegistEventHandler(TScriptInterface<IEventHandlerInterface> EventHandler);

	UFUNCTION(BlueprintCallable, Category = "EventSystem")
	void UnRegistEventHandler(TScriptInterface<IEventHandlerInterface> EventHandler);

	// Begin FTickableGameObject implementation
	virtual bool IsTickableWhenPaused() const override { return false; }
	virtual bool IsTickableInEditor() const override { return false; }
	virtual bool IsTickable() const override { return true; }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(EventSystem, STATGROUP_Tickables); }
	// End FTickableGameObject

    /** 给playercontroller调用来处理rpc的 **/
    void HandleSendEventToServer(class ACorePlayerController* PlayerController, const FString& EventClass, const FString& SerializeEvent);
    void HandleSendEventToClient(const FString& EventClass, const FString& SerializeEvent);

private:
	TQueue<UGameEventBase*> mEventQueue;
	TMap<UClass*, TArray<TScriptInterface<IEventHandlerInterface>>> mEventHandlerMap;
	const int TickHandleEventMax = 10;
};