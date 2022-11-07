#include "RewardWidgetManager.h"
#include "Base/Log/UE4LogImpl.h"
#include <assert.h>
#include "RewardWidgetFactory.h"
#include "Data/Reward/CoreReward.h"

TMap<FString, TSharedPtr<RewardWidgetFactory>> RewardWidgetManager::mFactoryMap;

void RewardWidgetManager::RegistRewardWidgetFactory(TSharedPtr<RewardWidgetFactory> Factory)
{
    FString RewardWidgetName = Factory->GetRewardWidgetName();
    if (mFactoryMap.Contains(RewardWidgetName)) {
        UE_LOG(GameCore, Error, TEXT("工厂已经注册:%s"), *RewardWidgetName);
        return;
    }
    mFactoryMap.Add(RewardWidgetName, Factory);
}

void RewardWidgetManager::UnregistRewardWidgetFactory(TSharedPtr<RewardWidgetFactory> Factory)
{
    FString RewardWidgetName = Factory->GetRewardWidgetName();
    mFactoryMap.Remove(RewardWidgetName);
}

TArray<TSharedPtr<RewardWidgetFactory>> RewardWidgetManager::GetAllFactories()
{
    TArray<TSharedPtr<RewardWidgetFactory>> factories;
    for (auto Iter = mFactoryMap.CreateIterator(); Iter; ++Iter)
    {
        factories.Add(Iter->Value);
    }
    return factories;
}

TSharedPtr<RewardWidgetFactory> RewardWidgetManager::GetFactoryByWidgetName(const FString& name)
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

TSharedPtr<RewardWidgetFactory> RewardWidgetManager::GetFactoryByWidgetName(TSubclassOf<class UCoreReward> RewardClass)
{
    for (auto Iter = mFactoryMap.CreateIterator(); Iter; ++Iter) {
        if (Iter->Value->GetRewardClass() == RewardClass) {
            return Iter->Value;
        }
    }
    return nullptr;
}