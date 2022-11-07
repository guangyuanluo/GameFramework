// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SBoxPanel.h"

/**
* 奖励控件工厂
*/
class GAMEFRAMEWORKEDITOR_API RewardWidgetFactory
{
public:
	/**
    * 奖励类型控件名字
    */
    virtual FString GetRewardWidgetName() = 0;

    /**
    * 得到要渲染的class类型
    */
    virtual TSubclassOf<class UCoreReward> GetRewardClass() = 0;
    /**
    * 创建一个控件
    */
    virtual TSharedPtr<class SRewardWidget> CreateRewardWidget(UObject* InOuter, class UCoreReward* Reward, SVerticalBox::FSlot& ParentSlot) = 0;
};