// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "CloseToNPCCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta = (DisplayName = "靠近NPC", Category = "通用条件"))
class GAMEFRAMEWORK_API UCloseToNPCCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 靠近哪个npc
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;

	/**
	* @brief 半径
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	float Radius;
};

/**
 * 靠近NPC条件进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UCloseToNPCConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	bool HaveComplete = false;

	virtual void OnInitialize() override;
	virtual void OnUninitialize() override;
	virtual bool IsComplete(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
	virtual void OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnCharacterTemplateUpdate(class ACorePlayerController* PlayerController);

	UFUNCTION()
	void OnCharacterMovementUpdate(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);

	void UpdateSatisfy();
};