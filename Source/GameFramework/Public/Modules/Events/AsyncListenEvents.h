// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncListenEvents.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvent, UCoreGameInstance*, gameInstance, UGameEventBase*, handleEvent);

/**
* 监听事件的异步函数
*/
UCLASS(meta = (HideThen = true))
class GAMEFRAMEWORK_API UAsyncListenEvents : public UBlueprintAsyncActionBase, public IEventHandlerInterface
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true", WorldContext = "WorldContextObject"))
	static UAsyncListenEvents* StartListen(UObject* WorldContextObject, const TArray<TSubclassOf<class UGameEventBase>>& InListenEvents);

public:
	/**
	* 事件回调
	*/
	UPROPERTY(BlueprintAssignable)
	FOnEvent OnEventCallback;

	/**
	* 监听的事件
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<TSubclassOf<class UGameEventBase>> ListenEvents;

	/**
	* 终止监听
	*/
	UFUNCTION(BlueprintCallable)
	void Abort();

	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
	virtual void OnEvent(UCoreGameInstance* gameInstance, UGameEventBase* handleEvent) override;
	/**************EventHandler interface define end*************/

#if WITH_EDITORONLY_DATA
public:
	FDelegateHandle EndPlayDelegateHandle;
private:
	void OnMapChange(uint32 MapChangeFlags);
#endif
};
