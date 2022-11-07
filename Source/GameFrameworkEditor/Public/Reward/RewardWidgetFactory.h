// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SBoxPanel.h"

/**
* �����ؼ�����
*/
class GAMEFRAMEWORKEDITOR_API RewardWidgetFactory
{
public:
	/**
    * �������Ϳؼ�����
    */
    virtual FString GetRewardWidgetName() = 0;

    /**
    * �õ�Ҫ��Ⱦ��class����
    */
    virtual TSubclassOf<class UCoreReward> GetRewardClass() = 0;
    /**
    * ����һ���ؼ�
    */
    virtual TSharedPtr<class SRewardWidget> CreateRewardWidget(UObject* InOuter, class UCoreReward* Reward, SVerticalBox::FSlot& ParentSlot) = 0;
};