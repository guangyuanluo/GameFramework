// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreConditionProgress_Event.h"
#include "CloseToNPCConditionProgress.generated.h"

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

	virtual void OnInitialize_Implementation() override;
	virtual void OnUninitialize_Implementation() override;
	virtual bool IsComplete_Implementation(bool& IsValid) override;
	/**************EventHandler interface define begin*************/
	virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
	/**************EventHandler interface define end*************/

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnCharacterTemplateUpdate(class ACorePlayerController* PlayerController);

	UFUNCTION()
	void OnCharacterMovementUpdate(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
};
