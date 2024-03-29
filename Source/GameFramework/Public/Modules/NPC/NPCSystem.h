// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/SystemBase.h"
#include "Modules/NPC/NPCInfo.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "NPCSystem.generated.h"

class ACoreCharacter;


/*
* @brief 用户管理
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UNPCSystem : public USystemBase {
public:
	GENERATED_BODY()

	/**
	* 添加NPC
	*/
	void AddNPC(ACoreCharacter* NPC);

	/**
	* 移除NPC
	*/
	void RemoveNPC(ACoreCharacter* NPC);

	/**
	* 通过id查到npc
	*/
	UFUNCTION(BlueprintCallable)
	bool FindNPCInfo(int32 NPCID, FNPCInfo& NPCInfo);

	/**
	* 尝试占有NPC ID
	*/
	UFUNCTION(BlueprintCallable)
	bool TryAcquireNPCByID(AActor* Owner, const TArray<int32>& NPCIDs, UObject* CustomInfo);

	/**
	* 尝试占有NPC ID
	*/
	UFUNCTION(BlueprintCallable)
	bool TryAcquireNPCByContainer(AActor* Owner, const TArray<FUnitIDContainer>& NPCIDContainers, UObject* CustomInfo);

	/**
	* 是否NPC空闲
	*/
	UFUNCTION(BlueprintCallable)
	bool IsNPCReleaseByID(const TArray<int32>& NPCIDs);

	/**
	* 是否NPC空闲
	*/
	UFUNCTION(BlueprintCallable)
	bool IsNPCReleaseByContainer(const TArray<FUnitIDContainer>& NPCIDs);

	/**
	* 通过NPC ID释放占有的npc
	*/
	UFUNCTION(BlueprintCallable)
	void ReleaseNPCByID(AActor* Owner, const TArray<int32>& NPCIDs);

	/**
	* 通过NPC ID释放占有的npc
	*/
	UFUNCTION(BlueprintCallable)
	void ReleaseNPCByContainer(AActor* Owner, const TArray<FUnitIDContainer>& NPCIDContainers);

private:
	UPROPERTY()
	TMap<int, FNPCInfo> NPCMap;
};