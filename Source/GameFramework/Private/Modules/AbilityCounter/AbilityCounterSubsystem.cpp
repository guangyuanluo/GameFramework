#include "AbilityCounterSubsystem.h"
#include "CoreAbility.h"

void UAbilityCounterSubsystem::SetCounterNum(const FString& CounterName, int CounterNum, class UCoreAbility* OwnerAbility) {
    FCounterInfo& CounterInfo = CounterMap.Add(CounterName);
    CounterInfo.Num = CounterNum;
    CounterInfo.OwnerAbility = OwnerAbility;
    if (OwnerAbility) {
        OwnerAbility->OnGameplayAbilityEndedWithData.AddUObject(this, &UAbilityCounterSubsystem::OnAbilityEnd, CounterName);
    }
    OnAbilityCounterNumChange.Broadcast(CounterName, CounterNum);
}

void UAbilityCounterSubsystem::ChangeCounterNum(const FString& CounterName, int DiffNum) {
    if (DiffNum == 0) {
        return;
    }
    auto FindCounterPtr = CounterMap.Find(CounterName);
    if (FindCounterPtr) {
        int NowNum = FindCounterPtr->Num + DiffNum;
        FindCounterPtr->Num = NowNum;
        OnAbilityCounterNumChange.Broadcast(CounterName, NowNum);
    }
}

void UAbilityCounterSubsystem::DestroyCounter(const FString& CounterName) {
    int Value = CounterMap.Remove(CounterName);
    if (Value != 0) {
        //不等于0，要通知计数器清0了
        OnAbilityCounterNumChange.Broadcast(CounterName, 0);
    }
}

int UAbilityCounterSubsystem::GetCounterNum(const FString& CounterName) const {
    auto FindCounterPtr = CounterMap.Find(CounterName);
    if (FindCounterPtr) {
        return FindCounterPtr->Num;
    }
    return 0;
}

void UAbilityCounterSubsystem::OnAbilityEnd(const FAbilityEndedData & AbilityEndedData, FString CounterName) {
    DestroyCounter(CounterName);
}