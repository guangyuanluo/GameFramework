// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreAbilitySystemComponent.generated.h"

/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */
UCLASS(ClassGroup = Skill, hidecategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), editinlinenew, meta = (BlueprintSpawnableComponent))
class GAMEFRAMEWORK_API UCoreAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
    /**
    * 从模板初始化技能
    */
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void InitSkillFromTemplate(int TemplateId);
};