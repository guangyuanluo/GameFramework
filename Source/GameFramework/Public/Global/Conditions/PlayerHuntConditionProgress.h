// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "PlayerHuntConditionProgress.generated.h"

/**
 * 玩家boss输出进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPlayerHuntConditionProgress : public UCoreConditionProgress_Event
{
public:
	GENERATED_BODY()

	/**
	* 输出贡献值
	*/
	UPROPERTY(Replicated, Category = "ConditionSystem", EditAnywhere, BlueprintReadOnly)
	float ContributionDegree;

	virtual bool IsComplete_Implementation() override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
