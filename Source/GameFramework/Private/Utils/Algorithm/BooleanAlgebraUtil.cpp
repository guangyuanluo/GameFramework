#include "BooleanAlgebraUtil.h"

TArray<TArray<int>> UBooleanAlgebraUtil::SplitRelationIndexArray(BooleanAlgebraEnum Relation, const TArray<BooleanAlgebraEnum>& LoopRelations, const TArray<int>& RelationIndexs) {
    TArray<TArray<int>> Result;

    TArray<int> SplitIndexArray;
    for (int Index = 0; Index < RelationIndexs.Num(); ++Index) {
        int RealIndex = RelationIndexs[Index];
        if (SplitIndexArray.Num() == 0) {
            SplitIndexArray.Add(RealIndex);
        }
        else {
            if (LoopRelations[RealIndex] != Relation) {
                SplitIndexArray.Add(RealIndex);
            }
            else {
                Result.Add(SplitIndexArray);
                SplitIndexArray.Empty();
                SplitIndexArray.Add(RealIndex);
            }
        }
    }
    //最后loop完，把剩下的添加
    if (SplitIndexArray.Num() > 0) {
        Result.Add(SplitIndexArray);
    }

    return Result;
}

void UBooleanAlgebraUtil::RelationsGenerateRecursive(FBooleanAlgebraNodeInfo& NodeInfo, const TArray<BooleanAlgebraEnum>& RelationOrders, int NowOrderIndex, const TArray<BooleanAlgebraEnum>& LoopRelations, const TArray<int>& RelationIndexs) {
    const auto& Relation = RelationOrders[NowOrderIndex];
    NodeInfo.IsRelationNode = true;
    NodeInfo.Relation = Relation;

    if (NowOrderIndex == 0) {
        //最后一层，不需要分割出更多的运算符
        if (RelationIndexs.Num() == 1) {
            NodeInfo.IsRelationNode = false;
            NodeInfo.Index = RelationIndexs[0];
        }
        else {
            for (const auto& Index : RelationIndexs) {
                FBooleanAlgebraNodeInfo& ChildRef = NodeInfo.Children.Add_GetRef(FBooleanAlgebraNodeInfo());
                ChildRef.Index = Index;
                ChildRef.IsRelationNode = false;
            }
        }
    }
    else {
        TArray<TArray<int>> SplitArray = SplitRelationIndexArray(Relation, LoopRelations, RelationIndexs);

        if (SplitArray.Num() == 1 && SplitArray[0].Num() == 1) {
            NodeInfo.IsRelationNode = false;
            NodeInfo.Index = SplitArray[0][0];
        }
        else {
            if (SplitArray.Num() == 1) {
                //没有分割，说明这层运算符只有一个，等于不需要运算，跳过，直接到下层
                RelationsGenerateRecursive(NodeInfo, RelationOrders, NowOrderIndex - 1, LoopRelations, SplitArray[0]);
            }
            else {
                for (int ArrayIndex = 0; ArrayIndex < SplitArray.Num(); ++ArrayIndex) {
                    FBooleanAlgebraNodeInfo& ChildRef = NodeInfo.Children.Add_GetRef(FBooleanAlgebraNodeInfo());
                    //继续往下一优先级递归
                    RelationsGenerateRecursive(ChildRef, RelationOrders, NowOrderIndex - 1, LoopRelations, SplitArray[ArrayIndex]);
                }
            }
        }
    }
}

FBooleanAlgebraNodeInfo UBooleanAlgebraUtil::RelationsGenerate(const TArray<BooleanAlgebraEnum>& LoopRelations) {
    FBooleanAlgebraNodeInfo Root;
    //逻辑运算优先级
    static const TArray<BooleanAlgebraEnum>& RelationOrders = { BooleanAlgebraEnum::E_XOR, BooleanAlgebraEnum::E_AND, BooleanAlgebraEnum::E_OR };

    TArray<int> IndexArray;
    for (int Index = 0; Index < LoopRelations.Num(); ++Index) {
        IndexArray.Add(Index);
    }

    RelationsGenerateRecursive(Root, RelationOrders, RelationOrders.Num() - 1, LoopRelations, IndexArray);

    return Root;
}

bool UBooleanAlgebraUtil::ExecuteConditionRelationTree(const FBooleanAlgebraNodeInfo& Node, const TFunction<bool(int)>& BooleanExpressionExecFunc) {
    bool FinalResult = true;

    if (Node.IsRelationNode) {
        for (int Index = 0; Index < Node.Children.Num(); ++Index) {
            bool ChildResult = true;
            const auto& Child = Node.Children[Index];
            if (Child.IsRelationNode) {
                ChildResult = ExecuteConditionRelationTree(Child, BooleanExpressionExecFunc);
            }
            else {
                ChildResult = BooleanExpressionExecFunc(Child.Index);
            }
            bool NeedStopLoop = false;
            if (Index == 0) {
                FinalResult = ChildResult;
            }
            else {
                switch (Node.Relation) {
                case BooleanAlgebraEnum::E_AND:
                    FinalResult = FinalResult && ChildResult;
                    break;
                case BooleanAlgebraEnum::E_OR:
                    FinalResult = FinalResult || ChildResult;
                    break;
                case BooleanAlgebraEnum::E_XOR:
                    FinalResult = FinalResult ^ ChildResult;
                    break;
                default:
                    check(false);
                    break;
                }
            }
            switch (Node.Relation) {
            case BooleanAlgebraEnum::E_AND:
                if (!FinalResult) {
                    //and运算符，一个为false，后面都不用再运算了
                    NeedStopLoop = true;
                }
                break;
            case BooleanAlgebraEnum::E_OR:
                if (FinalResult) {
                    //or运算符，一个为true，后面都不用再运算了
                    NeedStopLoop = true;
                }
                break;
            default:
                break;
            }
            if (NeedStopLoop) {
                break;
            }
        }
    }
    else {
        FinalResult = BooleanExpressionExecFunc(Node.Index);
    }
    return FinalResult;
}