// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/Structure/LogicObjectBase.h"
#include "FindEnemyBase.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract, Meta = (DisplayName = "索敌"))
class GAMEFRAMEWORK_API UFindEnemyBase : public ULogicObjectBase
{
	GENERATED_BODY()

public:
	/**
	* 查找敌人
	*/
	UFUNCTION(BlueprintNativeEvent)
	class ACoreCharacter* FindEnemy(class UFindEnemyComponent* OwnerComponent);
};