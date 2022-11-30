// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate/SSimpleRow.h"

/**
 * 
 */
class SGameFrameworkWidget_SkillGroupTab : public SSimpleRow
{
public:
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetRowSource() override;
    virtual class UDataTable* GetDataTable() override;
    virtual FName NewRowInit(FName Name) override;
	int32 ApplySkillGroupId();
protected:
    virtual bool AllowRowRemove(struct FConfigTableRowBase* RemoveRow) override;
};