// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "CoreItem.generated.h"

/**
 * 物品数据
 */
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UCoreItem : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Item")
    int32 ItemId;

    UPROPERTY(ReplicatedUsing = OnRep_ItemNum, BlueprintReadOnly, Category = "Item")
    int32 ItemNum;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAbilitySpecHandle ActiveAbilitySpecHandle;

    /**
    * 检查是不是自动激活，是的话就激活
    */
    UFUNCTION(BlueprintCallable)
    void CheckAndAutoActiveEffects(class UBackpackComponent* BackpackComponent);

    /**
    * 激活物品效果
    */
    UFUNCTION(BlueprintCallable)
    void ActiveEffects(class UBackpackComponent* BackpackComponent);

    /**
    * 反激活物品效果
    */
    UFUNCTION(BlueprintCallable)
    void DeactiveEffects(class UBackpackComponent* BackpackComponent);

    virtual bool IsSupportedForNetworking() const override;
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:
    UFUNCTION()
    void OnRep_ItemNum();

    void ActiveEffectsPrivate(class UBackpackComponent* BackpackComponent, const struct FItemConfigTableRow* ItemInfo);
};