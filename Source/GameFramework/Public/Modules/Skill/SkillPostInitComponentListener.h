// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillPostInitComponentListener.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USkillPostInitComponentListener : public UInterface
{
	GENERATED_BODY()
};

/**
 * 挂在角色组件上，监听技能初始化完成，做一些自己的初始化
 */
class GAMEFRAMEWORK_API ISkillPostInitComponentListener
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
    void PostSkillTemplateInit();
};
