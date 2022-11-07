// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "BackpackExtendHandler.generated.h"

/*
* @brief 背包扩展处理
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UBackpackExtendHandler : public UObject {
    GENERATED_BODY()

public:
    /**
    * 物品加入背包处理
    */
    UFUNCTION(BlueprintNativeEvent)
    void OnItemAdd(class UBackpackComponent* BackpackComponent, class UCoreItem* Item, uint8 BackpackType, int PackageIndex);

    /**
    * 物品移出背包处理
    */
    UFUNCTION(BlueprintNativeEvent)
    void OnItemRemove(class UBackpackComponent* BackpackComponent, class UCoreItem* Item, uint8 BackpackType, int PackageIndex);

    /**
    * 物品效果增加前处理
    */
    UFUNCTION(BlueprintNativeEvent)
    void PreItemEffectAdd(const FGameplayEffectSpecHandle& Spec, TSubclassOf<class UGameplayEffect> EffectClass, class UBackpackComponent* BackpackComponent, class UAbilitySystemComponent* AbilitySystemComponent, class UCoreItem* Item);
};