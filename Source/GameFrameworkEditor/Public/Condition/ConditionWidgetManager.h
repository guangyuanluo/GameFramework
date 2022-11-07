// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class ConditionWidgetFactory;

class GAMEFRAMEWORKEDITOR_API ConditionWidgetManager {
public:
	static void RegistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory);
	static void UnregistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory);
	static TArray<TSharedPtr<ConditionWidgetFactory>> GetAllFactories();
	static TSharedPtr<ConditionWidgetFactory> GetFactoryByWidgetName(const FString& name);
    static TSharedPtr<ConditionWidgetFactory> GetFactoryByWidgetName(TSubclassOf<class UCoreCondition> conditionClass);

private:
	static TMap<FString, TSharedPtr<ConditionWidgetFactory>> mFactoryMap;
};