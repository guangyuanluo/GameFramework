// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class ConditionWidgetFactory;
class UCoreCondition;

class GAMEFRAMEWORKEDITOR_API ConditionWidgetManager {
public:
	static void RegistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory);
	static void UnregistConditionWidgetFactory(TSharedPtr<ConditionWidgetFactory> Factory);
	static TArray<TSharedPtr<ConditionWidgetFactory>> GetAllFactories();
    static TSharedPtr<ConditionWidgetFactory> GetFactoryByConditionClass(UClass* ConditionClass);

private:
	static TMap<TSubclassOf<UCoreCondition>, TSharedPtr<ConditionWidgetFactory>> FactoryMap;
};