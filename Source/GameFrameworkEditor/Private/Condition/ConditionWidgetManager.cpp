#include "ConditionWidgetManager.h"
#include "Base/Log/UE4LogImpl.h"
#include <assert.h>
#include "ConditionWidgetFactory.h"
#include "Modules/Condition/CoreCondition.h"

TMap<TSubclassOf<UCoreCondition>, TSharedPtr<ConditionWidgetFactory>> ConditionWidgetManager::FactoryMap;

void ConditionWidgetManager::RegistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory)
{
    TSubclassOf<UCoreCondition> ConditionClass = Factory->GetConditionClass();
    if (FactoryMap.Contains(ConditionClass)) {
        UE_LOG(GameCore, Error, TEXT("工厂已经注册:%s"), *ConditionClass->GetDisplayNameText().ToString());
        return;
    }
    FactoryMap.Add(ConditionClass, Factory);
}

void ConditionWidgetManager::UnregistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory)
{
    TSubclassOf<UCoreCondition> ConditionClass = Factory->GetConditionClass();
    FactoryMap.Remove(ConditionClass);
}

TArray<TSharedPtr<ConditionWidgetFactory>> ConditionWidgetManager::GetAllFactories()
{
    TArray<TSharedPtr<ConditionWidgetFactory>> Factories;
    for (auto Iter = FactoryMap.CreateIterator(); Iter; ++Iter)
    {
        Factories.Add(Iter->Value);
    }
    return Factories;
}

TSharedPtr<ConditionWidgetFactory> ConditionWidgetManager::GetFactoryByConditionClass(UClass* ConditionClass)
{
    auto FindFactory = FactoryMap.Find(ConditionClass);
    if (FindFactory) {
        return *FindFactory;
    }
    return nullptr;
}