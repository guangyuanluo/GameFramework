// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameEntity.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UGameEntity : public UInterface
{
	GENERATED_BODY()
};

/**
 * 游戏实体接口
 */
class GAMEFRAMEWORK_API IGameEntity
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	* 获得实体id
	*/
	UFUNCTION(BlueprintCallable)
	virtual const FString& GetEntityID();

	/**
	* 设置实体id
	*/
	UFUNCTION(BlueprintCallable)
	virtual void SetEntityID(const FString& EntityID);

    /**
    * 得到位置
    */
    UFUNCTION(BlueprintCallable)
    virtual const FTransform& GetTransform() const;

    /**
    * 设置transform
    */
    UFUNCTION(BlueprintCallable)
    virtual void SetTransform(const FTransform& NewTransform);

	/** 接口实现 */
	virtual FString& EntityID_Implementation() = 0;
};
