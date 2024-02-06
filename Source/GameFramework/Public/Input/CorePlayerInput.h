// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "EnhancedPlayerInput.h"
#include "EnhancedActionKeyMapping.h"
#include "CorePlayerInput.generated.h"


UCLASS()
class GAMEFRAMEWORK_API UCorePlayerInput : public UEnhancedPlayerInput
{
	GENERATED_BODY()

public:	
	const TArray<FEnhancedActionKeyMapping>& GetEnhancedActionMappingsConst();
};
