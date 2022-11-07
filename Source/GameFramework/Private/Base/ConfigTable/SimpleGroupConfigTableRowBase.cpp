// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleGroupConfigTableRowBase.h"

TArray<int> Constant;

TArray<int>& FSimpleGroupConfigTableRowBase::GetGroupItems() {
    return Constant;
}