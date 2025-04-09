// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CoreCharacterStateBase.generated.h"

/**
 * 基础状态类
 */
UCLASS()
class GAMEFRAMEWORK_API ACoreCharacterStateBase : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
    ACoreCharacterStateBase(const class FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterState")
    class UBackpackComponent* BackpackComponent;

    UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    class UExpComponent* ExpComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "CharacterState", Replicated)
    class UCoreAbilitySystemComponent* SkillComponent;

    UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    class UQuestComponent* QuestComponent;

    UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    class UPlayerComponent* PlayerComponent;

    UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    class UWalletComponent* WalletComponent;

    UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    class UScenarioComponent* ScenarioComponent;

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
protected:
};