// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class RewardWidgetFactory;

class GAMEFRAMEWORKEDITOR_API RewardWidgetManager {
public:
	static void RegistRewardWidgetFactory(TSharedPtr<RewardWidgetFactory> Factory);
	static void UnregistRewardWidgetFactory(TSharedPtr<RewardWidgetFactory> Factory);
	static TArray<TSharedPtr<RewardWidgetFactory>> GetAllFactories();
	static TSharedPtr<RewardWidgetFactory> GetFactoryByWidgetName(const FString& name);
    static TSharedPtr<RewardWidgetFactory> GetFactoryByWidgetName(TSubclassOf<class UCoreReward> RewardClass);

private:
	static TMap<FString, TSharedPtr<RewardWidgetFactory>> mFactoryMap;
};