// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate/SSimpleGroupBase.h"
/**
 *
 */
class SGameFrameworkWidget_StoreTab : public SSimpleGroupBase {
public:
    virtual UDataTable* GetGroupDataTable() override;
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetGroupItemSource() override;
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetGroupSource() override;
    virtual FName NewRowInit(FName Name) override;
    int32 ApplyId();
protected:
    virtual bool GroupTableRowPreRemove(FName Name) override;
};