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
    * 得到要渲染的class类型
    */
    virtual TSubclassOf<class UCoreCondition> GetConditionClass() = 0;
    /**
    * 是否允许创建该条件
    */
    virtual bool CanCreateCondition() = 0;

    /**
    * 对创建出来的条件进行一些初始化处理
    */
    virtual void PostInitConditionCreated(class UCoreCondition* Condition) = 0;
};