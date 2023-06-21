// Fill out your copyright notice in the Description page of Project Settings.
#include "SConditionWidget.h"
#include "Modules/Condition/CoreCondition.h"

SConditionWidget::~SConditionWidget() {}

void SConditionWidget::Construct(const FArguments& InArgs, UCoreCondition* InWidgetCondition, int ChildIndex) {
    WidgetCondition = InWidgetCondition;
}