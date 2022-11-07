// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SBoxPanel.h"

/**
* 条件控件工厂
*/
class GAMEFRAMEWORKEDITOR_API ConditionWidgetFactory
{
public:
	/**
    * 条件类型控件名字
    */
    virtual FString GetConditionWidgetName() = 0;

    /**
    * 得到要渲染的class类型
    */
    virtual TSubclassOf<class UCoreCondition> GetConditionClass() = 0;
    /**
    * 创建一个控件
    */
    virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* condition, SVerticalBox::FSlot& parentSlot) = 0;
};