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
	UFUNCTION(BlueprintNativeEvent, Category = "EventSystem")
	TArray<UClass*> GetHandleEventTypes();

	/*
	* 处理event实现
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "EventSystem")
	void OnEvent(UCoreGameInstance* gameInstance, UGameEventBase* handleEvent);
};