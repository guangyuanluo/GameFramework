// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSortPredicate.generated.h"

/*
* @brief 背包物品排序优先级
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UItemSortPredicate : public UObject {
    GENERATED_BODY()

public:
    /**
    * 物品排序优先级
    */
    UFUNCTION(BlueprintNativeEvent)
    bool Compare(int itemIdA, int itemIdB);
};