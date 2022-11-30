// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Modules/Skill/CoreAbilitySystemComponent.h"
#include "CoreItem.generated.h"

DECLARE_DYNAMIC_DELEGATE_FourParams(FItemEffectPreAddDelegate, class UCoreAbilitySystemComponent*, SkillComponent, const FGameplayEffectSpecHandle&, Spec, TSubclassOf<class UGameplayEffect>, EffectClass, UCoreItem*, Item);

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

    /**
    * 配置在基类上的效果，免除重复配表
    */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (DisplayName = "基类效果"))
    TArray<FEffectInfo> Effects;

    /**
    * 激活物品所有效果
    */
    UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InEffectPreAddCallback"))
    void ActiveEffects(class UBackpackComponent* BackpackComponent, const FItemEffectPreAddDelegate& InEffectPreAddCallback);

    /**
    * 激活父类Actor上的效果
    */
    UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InEffectPreAddCallback"))
    void ActiveActorEffects(class UBackpackComponent* BackpackComponent, const FItemEffectPreAddDelegate& InEffectPreAddCallback);

    /**
    * 激活item表里配置的效果
    */
    UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "InEffectPreAddCallback"))
    void ActiveItemTableEffects(class UBackpackComponent* BackpackComponent, const FItemEffectPreAddDelegate& InEffectPreAddCallback);

    /**
    * 反激活物品效果
    */
    UFUNCTION(BlueprintCallable)
    void DeactiveEffects(class UBackpackComponent* BackpackComponent);

    /**
    * 反激活父类Actor上的效果
    */
    UFUNCTION(BlueprintCallable)
    void DeactiveActorEffects(class UBackpackComponent* BackpackComponent);

    /**
    * 反激活父类Actor上的效果
    */
    UFUNCTION(BlueprintCallable)
    void DeactiveItemTableEffects(class UBackpackComponent* BackpackComponent);

    virtual bool IsSupportedForNetworking() const override;
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:
    UFUNCTION()
    void OnRep_ItemNum();

    void ActiveActorEffectsPrivate(class UBackpackComponent* BackpackComponent, const FEffectPreAddDelegate& Callback);
    void ActiveItemTableEffectsPrivate(class UBackpackComponent* BackpackComponent, const FEffectPreAddDelegate& Callback);
};