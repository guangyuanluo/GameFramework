// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Exp/ExpInfo.h"
#include "HeroInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UHeroInfo : public UObject
{
public:
	GENERATED_BODY()

	UPROPERTY(Category = "HeroInfo", EditAnywhere, BlueprintReadWrite)
	FString HeroName;

	UPROPERTY(Category = "HeroInfo", EditAnywhere, BlueprintReadWrite)
	int UnitId;

	UPROPERTY(Category = "HeroInfo", EditAnywhere, BlueprintReadWrite)
	TArray<FExpInfo> Exps;

	UPROPERTY(Category = "HeroInfo", EditAnywhere, BlueprintReadWrite)
	TArray<int> EquipItems;

	virtual void Serialize(FArchive& Ar) override;
};