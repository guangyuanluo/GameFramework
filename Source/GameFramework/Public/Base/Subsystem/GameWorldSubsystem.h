// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/EngineTypes.h"
#include "GameWorldSubsystem.generated.h"

/**
 * world子系统，暴露给蓝图
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEFRAMEWORK_API UGameWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subsystem", DisplayName = "限制的World网络类型", Meta = (ToolTip = "0.不限制 1.Client 2.Server"))
	uint8 LimitNetMode = 0;

	/**
	* 初始化
	*/
	UFUNCTION(BlueprintNativeEvent)
	void Init();

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};