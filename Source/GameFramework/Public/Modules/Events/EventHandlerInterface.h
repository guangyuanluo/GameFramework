// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "EventHandlerInterface.generated.h"

class UCoreGameInstance;
class UGameEventBase;

UINTERFACE()
class GAMEFRAMEWORK_API UEventHandlerInterface : public UInterface {
	GENERATED_BODY()
};
/**
 * 
 */
class GAMEFRAMEWORK_API IEventHandlerInterface {
	GENERATED_BODY()
public:

	/*
	* 获取处理的event type列表
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "EventSystem", DisplayName = "GetHandleEventTypes", meta = (ScriptName = "GetHandleEventTypes"))
	TArray<TSubclassOf<class UGameEventBase>> K2_GetHandleEventTypes();

	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes();

	/*
	* 处理event实现
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "EventSystem", DisplayName = "OnEvent", meta = (ScriptName = "OnEvent"))
	void K2_OnEvent(UCoreGameInstance* GameInstance, UGameEventBase* HandleEvent);

	virtual void OnEvent(UCoreGameInstance* GameInstance, UGameEventBase* HandleEvent);
};