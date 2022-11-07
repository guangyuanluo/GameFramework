// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SortUtils.generated.h"

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API USortUtils : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

    /**
    * 用插入排序, 低优先级的排前面，高优先级的排后面
    * Compare表示A元素与B元素比较，A比B低返回true，A比B高返回false
    */
	template<class T>
	static void SortArray(TArray<T*>& InArray, TFunction<bool(T* A, T* B)>& CompareFunc) {
        TArray<TLinkedList<T*>*> AllNodes;
        TLinkedList<T*>* Head = nullptr;
        for (int Index = 0; Index < InArray.Num(); ++Index) {
            if (InArray[Index]) {
                if (AllNodes.Num() == 0) {
                    TLinkedList<T*>* First = new TLinkedList<T*>(InArray[Index]);
                    AllNodes.Add(First);
                    Head = AllNodes[0];
                }
                else {
                    bool Handle = false;
                    TLinkedList<T*>* LastIter = nullptr;

                    T* Item = InArray[Index];
                    auto Iter = Head;
                    while (Iter) {
                        auto IterItem = (**Iter);
                        if (Item != IterItem) {
                            //这里表示对象不一样，用优先级排序
                            if (CompareFunc(Item, IterItem)) {
                                auto NextIter = Iter->Next();
                                if (!NextIter) {
                                    LastIter = Iter;
                                }
                                Iter = NextIter;
                            }
                            else {
                                //比较结果为false，说明优先级低
                                TLinkedList<T*>* NewNode = new TLinkedList<T*>(InArray[Index]);
                                AllNodes.Add(NewNode);
                                auto NewNodePtr = AllNodes[AllNodes.Num() - 1];
                                auto BeforeNode = Iter->GetPrevLink();
                                NewNodePtr->LinkBefore(Iter);
                                if (Iter == Head) {
                                    //更新头节点
                                    Head = NewNodePtr;
                                }
                                Handle = true;
                                break;
                            }
                        }
                    }
                    if (!Handle) {
                        //没有处理，应该是到最后，所以加到最后
                        TLinkedList<T*>* NewNode = new TLinkedList<T*>(InArray[Index]);
                        AllNodes.Add(NewNode);
                        auto NewNodePtr = AllNodes[AllNodes.Num() - 1];
                        NewNodePtr->LinkAfter(LastIter);
                    }
                }
            }
        }
        //这里完成了排序，先置空，然后重新进行赋值
        if (Head) {
            for (int Index = 0; Index < InArray.Num(); ++Index) {
                InArray[Index] = nullptr;
            }
            //高优先级的在后面，所以倒着赋值
            auto Iter = Head;
            for (int Index = AllNodes.Num() - 1; Index >= 0; --Index) {
                InArray[Index] = **Iter;
                Iter = Iter->Next();
            }
            //释放
            for (int NodeIndex = 0; NodeIndex < AllNodes.Num(); ++NodeIndex) {
                delete AllNodes[NodeIndex];
            }
        }
	}
};