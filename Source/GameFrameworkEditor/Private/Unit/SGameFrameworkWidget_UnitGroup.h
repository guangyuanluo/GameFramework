// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSimpleGroupBase.h"

/**
 * 
 */
class SGameFrameworkWidget_UnitGroup : public SSimpleGroupBase
{
public:
    virtual UDataTable* GetGroupDataTable() override;
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetGroupItemSource() override;
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetGroupSource() override;
    virtual FName NewRowInit(FName Name) override;

private:
	int32 ApplyUnitGroupId();
};