// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameEventUtils.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API UGameEventUtils : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "EventSystem", meta = (WorldContext = "WorldContextObject"))
    static void PushEvent(UObject* WorldContextObject, class UGameEventBase* InPushEvent);

    UFUNCTION(BlueprintCallable, Category = "EventSystem", meta = (WorldContext = "WorldContextObject"))
    static void PushEventToServer(UObject* WorldContextObject, class UGameEventBase* InPushEvent, bool Reliable);

    UFUNCTION(BlueprintCallable, Category = "EventSystem", meta = (WorldContext = "WorldContextObject"))
    static void PushEventToClient(UObject* WorldContextObject, class ACorePlayerController* PlayerController, class UGameEventBase* InPushEvent);
};