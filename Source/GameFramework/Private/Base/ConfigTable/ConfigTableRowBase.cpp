// Fill out your copyright notice in the Description page of Project Settings.

#include "ConfigTableRowBase.h"

int FConfigTableRowBase::GetUniqueId() {
    return -1;
}

FString FConfigTableRowBase::GetSimpleDescription() {
    return TEXT("非法数据");
}

void FConfigTableRowBase::CustomInit() {

}