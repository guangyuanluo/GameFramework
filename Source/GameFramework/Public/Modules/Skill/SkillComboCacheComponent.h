// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComboCacheComponent.generated.h"

/**
* 连招缓存的组件
*/
UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEFRAMEWORK_API USkillComboCacheComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentSectionName(FName NewSectionName);

	UFUNCTION(BlueprintPure)
	FName GetCurrentSectionName() const;
private:
	UPROPERTY()
	FName CurrentSectionName;
};