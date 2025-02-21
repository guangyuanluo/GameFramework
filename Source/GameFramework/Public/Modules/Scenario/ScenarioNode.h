// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScenarioNode.generated.h"

class UScenario;

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FScenarioNodeOutputPinInfo {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool AllowMulti;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract, config = Game)
class GAMEFRAMEWORK_API UScenarioNode : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
	class UScenario* ScenarioAsset;

	UPROPERTY()
	class UScenarioNode* PreScenarioNode;

	UPROPERTY()
	TArray<class UScenarioNode*> FollowScenarioNodes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, meta = (DisplayName = "动作列表"))
    TArray<TObjectPtr<class UScenarioAction>> Actions;

    UFUNCTION(BlueprintCallable)
    const TArray<class UScenarioNode*>& GetFollowScenarioNodes() const;

    UFUNCTION(BlueprintNativeEvent)
	FString GetNodeTitle();

    UFUNCTION(BlueprintNativeEvent)
    TArray<FScenarioNodeOutputPinInfo> GetOutputPinInfo();
};