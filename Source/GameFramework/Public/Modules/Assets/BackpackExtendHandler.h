// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Base/Structure/LogicObjectBase.h"
#include "Modules/Assets/BackpackTypes.h"
#include "BackpackExtendHandler.generated.h"

/*
* @brief 背包扩展处理
*/
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UBackpackExtendHandler : public ULogicObjectBase {
    GENERATED_BODY()

public:
    /**
     * 是否允许添加物品
     */
    UFUNCTION(BlueprintNativeEvent)
    bool AllowItemAdd(class UBackpackComponent* BackpackComponent, int ItemID, EBackpackTypeEnum BackpackType);
    /**
    * 背包物品变动处理
    */
    UFUNCTION(BlueprintNativeEvent)
    void OnItemChange(class UBackpackComponent* BackpackComponent, class UCoreItem* NewItem, UCoreItem* OldItem, EBackpackTypeEnum BackpackType, int PackageIndex);

    /**
    * 物品效果增加前处理
    */
    UFUNCTION(BlueprintNativeEvent)
    void PreItemEffectAdd(const FGameplayEffectSpecHandle& Spec, TSubclassOf<class UGameplayEffect> EffectClass, class UBackpackComponent* BackpackComponent, class UAbilitySystemComponent* AbilitySystemComponent, class UCoreItem* Item);

    /**
    * 能否移动物品
    */
    UFUNCTION(BlueprintNativeEvent)
    bool CanMoveItem(class UBackpackComponent* BackpackComponent, EBackpackTypeEnum SourceBackpackType, int SourceSlotIndex, UCoreItem* SourceItem, EBackpackTypeEnum TargetBackpackType, int TargetSlotIndex, UCoreItem* TargetItem);
};