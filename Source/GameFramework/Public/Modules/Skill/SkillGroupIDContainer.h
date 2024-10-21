// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillGroupIDContainer.generated.h"

/**
* 技能组ID容器
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FSkillGroupIDContainer
{
	GENERATED_BODY()

	/**
	*  技能组ID
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "单位ID"))
	int32 SkillGroupID = 0;
};