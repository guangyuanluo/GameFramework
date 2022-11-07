// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate/SSimpleRow.h"

/**
 * 
 */
class SGameFrameworkWidget_Unit : public SSimpleRow
{
public:
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetRowSource() override;
    virtual class UDataTable* GetDataTable() override;
    virtual FName NewRowInit(FName Name) override;
protected:
	int32 ApplyUnitId();
    virtual TSharedRef<ITableRow> ListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable) override;
    virtual TSharedRef<class SHeaderRow> ConstructListViewHeadRow() override;
    virtual bool AllRowModified(FName Name, struct FConfigTableRowBase* ChangeRow, struct FConfigTableRowBase* OriginRow) override;
    virtual bool AllowRowRemove(struct FConfigTableRowBase* RemoveRow) override;
    virtual void RowRemoved(struct FConfigTableRowBase* RemoveRow) override;
};