// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "CoreGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API ACoreGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	FUnitIDContainer DebugStartTemplate;

	/** 玩家登陆前的校验 **/
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	/**玩家登陆*/
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	/** 玩家成功登陆的回调 **/
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/** 玩家登出 **/
	virtual void Logout(AController* Exiting) override;
	/** 获取玩家模板 */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};