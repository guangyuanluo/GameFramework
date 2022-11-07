// Fill out your copyright notice in the Description page of Project Settings.
#include "SRewardWidget.h"
#include "Data/Reward/CoreReward.h"

SRewardWidget::~SRewardWidget() {}

void SRewardWidget::Construct(const FArguments& InArgs, UCoreReward* InWidgetReward) {
    WidgetReward = InWidgetReward;
}