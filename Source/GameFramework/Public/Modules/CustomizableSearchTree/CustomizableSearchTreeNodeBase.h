// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomizableSearchTreeNodeBase.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FCustomizableSearchTreeNodeOutputPinInfo {
    GENERATED_BODY()

    /**
     * 是否允许连接多个
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool AllowMulti;

    /**
     * 显示名字
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name;
};

/**
 * 定制化搜索树节点基类
 */
UCLASS(BlueprintType, Blueprintable, Abstract, config = Game)
class GAMEFRAMEWORK_API UCustomizableSearchTreeNodeBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UCustomizableSearchTree* Asset;

	UPROPERTY()
	class UCustomizableSearchTreeNodeBase* PreNode;

	UPROPERTY()
	TArray<class UCustomizableSearchTreeNodeBase*> FollowNodes;

    UFUNCTION(BlueprintNativeEvent)
	FString GetNodeTitle();

    UFUNCTION(BlueprintNativeEvent)
    TArray<FCustomizableSearchTreeNodeOutputPinInfo> GetOutputPinInfo();
};