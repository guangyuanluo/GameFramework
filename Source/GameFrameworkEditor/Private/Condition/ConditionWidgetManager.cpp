#include "ConditionWidgetManager.h"
#include "Base/Log/UE4LogImpl.h"
#include <assert.h>
#include "ConditionWidgetFactory.h"
#include "Modules/Condition/CoreCondition.h"

TMap<FString, TSharedPtr<ConditionWidgetFactory>> ConditionWidgetManager::mFactoryMap;

void ConditionWidgetManager::RegistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory)
{
    FString conditionWidgetName = Factory->GetConditionWidgetName();
    if (mFactoryMap.Contains(conditionWidgetName)) {
        UE_LOG(GameCore, Error, TEXT("工厂已经注册:%s"), *conditionWidgetName);
        return;
    }
    mFactoryMap.Add(conditionWidgetName, Factory);
}

void ConditionWidgetManager::UnregistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory)
{
    FString conditionWidgetName = Factory->GetConditionWidgetName();
    mFactoryMap.Remove(conditionWidgetName);
}

TArray<TSharedPtr<ConditionWidgetFactory>> ConditionWidgetManager::GetAllFactories()
{
    TArray<TSharedPtr<ConditionWidgetFactory>> factories;
    for (auto Iter = mFactoryMap.CreateIterator(); Iter; ++Iter)
    {
        factories.Add(Iter->Value);
    }
    return factories;
}

TSharedPtr<ConditionWidgetFactory> ConditionWidgetManager::GetFactoryByWidgetName(const FString& name)
{
    auto findPtr = mFactoryMap.Find(name);
    if (findPtr)
    {
        return *findPtr;
    }
    else
    {
        return nullptr;
    }
}

TSharedPtr<ConditionWidgetFactory> ConditionWidgetManager::GetFactoryByWidgetName(TSubclassOf<class UCoreCondition> conditionClass)
{
    for (auto Iter = mFactoryMap.CreateIterator(); Iter; ++Iter) {
        if (Iter->Value->GetConditionClass() == conditionClass) {
            return Iter->Value;
        }
    }
    return nullptr;
}