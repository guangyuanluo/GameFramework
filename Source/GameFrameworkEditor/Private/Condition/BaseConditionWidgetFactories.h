// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class BaseConditionWidgetFactories {
public:
    static void Init();
    static void Uninit();

private:
    static TArray<TSharedPtr<class ConditionWidgetFactory>> BaseFactories;
};