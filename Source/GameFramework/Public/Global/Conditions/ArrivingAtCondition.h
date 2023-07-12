// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "Modules/Unit/UnitIDContainer.h"
#include "ArrivingAtCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta = (DisplayName = "到达地点", Category = "通用条件"))
class GAMEFRAMEWORK_API UArrivingAtCondition : public UCoreCondition
{
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 指定到达的单位id，即UnitInfo的id
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FUnitIDContainer UnitIDContainer;

	/**
	* @brief 得到移动到的名字
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite)
	FString LocationName;
};

/**
 * 到达地点人物条件
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UArrivingAtConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 是否完成
	*/
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	bool HaveComplete;

	virtual void OnInitialize_Implementation() override;
	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};