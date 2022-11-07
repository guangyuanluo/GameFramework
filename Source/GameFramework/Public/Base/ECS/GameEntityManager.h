// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntity.h"
#include "GameEntityManager.generated.h"

/*
* @brief 实体管理器
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UGameEntityManager : public UObject {
public:
	GENERATED_BODY()

	/**
	* 根据id返回尸体
	*/
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IGameEntity> GetEntityById(const FString& EntityId);

	/** 内部调用，实体自己注册 */
	void OnEntityAdd(TScriptInterface<IGameEntity> Entity);
	void OnEntityRemove(TScriptInterface<IGameEntity> Entity);
	void OnEntityIDChanged(TScriptInterface<IGameEntity> Entity, const FString& OldID);

private:
    UPROPERTY()
	TMap<FString, TScriptInterface<IGameEntity>> EntityMap;
};