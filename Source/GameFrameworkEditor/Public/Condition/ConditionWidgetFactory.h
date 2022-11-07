// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SBoxPanel.h"

/**
* �����ؼ�����
*/
class GAMEFRAMEWORKEDITOR_API ConditionWidgetFactory
{
public:
	/**
    * �������Ϳؼ�����
    */
    virtual FString GetConditionWidgetName() = 0;

    /**
    * �õ�Ҫ��Ⱦ��class����
    */
    virtual TSubclassOf<class UCoreCondition> GetConditionClass() = 0;
    /**
    * ����һ���ؼ�
    */
    virtual TSharedPtr<class SConditionWidget> CreateConditionWidget(UObject* Outer, class UCoreCondition* condition, SVerticalBox::FSlot& parentSlot) = 0;
};