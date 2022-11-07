// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/SystemBase.h"
#include "PlayerSystem.generated.h"

/*
* @brief 用户管理
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerSystem : public USystemBase {
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	TMap<FString, class ACorePlayerController*> GamePlayers;
};