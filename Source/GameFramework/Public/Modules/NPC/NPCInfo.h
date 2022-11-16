// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCInfo.generated.h"

USTRUCT(BlueprintType)
struct FNPCInfo {
	GENERATED_BODY()

	/**
	* 对象
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class ACoreCharacter* NPC;

	/**
	* 是否释放，空闲中
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsReleased = true;

	/**
	* 被占有的自定义信息
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UObject* CustomInfo;
};