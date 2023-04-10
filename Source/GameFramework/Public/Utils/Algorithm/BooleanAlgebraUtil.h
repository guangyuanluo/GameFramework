// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BooleanAlgebraUtil.generated.h"

/**
 * 布尔运算关系符
 */
UENUM(BlueprintType)
enum class BooleanAlgebraEnum :uint8 {
    E_AND UMETA(DisplayName = "与"),
    E_OR UMETA(DisplayName = "或"),
    E_XOR UMETA(DisplayName = "异或"),
};

/**
* 布尔条件节点信息
*/
struct GAMEFRAMEWORK_API FBooleanAlgebraNodeInfo {
    bool IsRelationNode = false;
    BooleanAlgebraEnum Relation;
    int Index;
    TArray<FBooleanAlgebraNodeInfo> Children;
};

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API UBooleanAlgebraUtil : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

    
    /** 构建运算符逻辑树 */
    static FBooleanAlgebraNodeInfo RelationsGenerate(const TArray<BooleanAlgebraEnum>& LoopRelations);
    /** 递归一颗逻辑树的执行结果 */
    static bool ExecuteConditionRelationTree(const FBooleanAlgebraNodeInfo& Node, const TFunction<bool(int)>& ConditionCheckFunc);

private:
    /** 按照运算符分割索引 */
    static TArray<TArray<int>> SplitRelationIndexArray(BooleanAlgebraEnum Relation, const TArray<BooleanAlgebraEnum>& LoopRelations, const TArray<int>& RelationIndexs);
    /** 按运算符优先级递归构建逻辑树 */
    static void RelationsGenerateRecursive(FBooleanAlgebraNodeInfo& NodeInfo, const TArray<BooleanAlgebraEnum>& RelationOrders, int NowOrderIndex, const TArray<BooleanAlgebraEnum>& LoopRelations, const TArray<int>& RelationIndexs);
};