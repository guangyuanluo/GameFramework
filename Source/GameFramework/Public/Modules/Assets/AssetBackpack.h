// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Assets/BackpackTypes.h"
#include "AssetBackpack.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FAssetBackpack {
    GENERATED_BODY()

    FAssetBackpack();

    UPROPERTY(Category = "AssetSystem", EditAnywhere, BlueprintReadOnly)
    EBackpackTypeEnum BackpackType;

	UPROPERTY(Category = "AssetSystem", EditAnywhere, BlueprintReadOnly)
	TArray<class UCoreItem*> ItemList;
};

UCLASS()
class GAMEFRAMEWORK_API UAssetBackpackBlueprintLibrary : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure)
    static bool IsValid(UPARAM(ref)FAssetBackpack& AssetBackpack);
};