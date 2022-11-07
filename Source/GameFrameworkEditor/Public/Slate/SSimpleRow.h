// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/SCompoundWidget.h"
#include "EditorCommon.h"

/**
 * 
 */
class GAMEFRAMEWORKEDITOR_API SSimpleRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSimpleRow)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList);
	virtual ~SSimpleRow() override;
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetRowSource() = 0;
    virtual class UDataTable* GetDataTable() = 0;
    virtual FName NewRowInit(FName Name) = 0;

protected:
    TSharedPtr<class SGameDataTableRowEditor> EditorRow;

    virtual void ConstructWidgetSources();
    virtual bool AllRowModified(FName Name, struct FConfigTableRowBase* ChangeRow, struct FConfigTableRowBase* OriginRow);
    virtual TSharedRef<ITableRow> ListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable);
    virtual TSharedRef<class SHeaderRow> ConstructListViewHeadRow();
    virtual bool AllowRowRemove(struct FConfigTableRowBase* RemoveRow);
    virtual void RowRemoved(struct FConfigTableRowBase* RemoveRow);

private:
	/**相关变量**/
	TSharedPtr<SListView<TSharedPtr<FConfigTableRowWrapper>>> ListView;
	TArray<TSharedPtr<FConfigTableRowWrapper>> Source;
	FName HighlightedRowName;
	/**相关变量**/

	/**-----------------界面相关---------------------**/
	//总界面
	TSharedRef<SWidget> ConstructPage();
	TSharedRef<SWidget> ConstructRowEditor();
	FName TableRowAdded(FName Name);
	bool TableRowPreRemove(FName Name);
	void TableRowRemoved(FName Name);
	void TableRowModified(FName Name);
	void TableSetHighlightedRow(FName Name);
	void OnSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo);
	/**-----------------界面相关---------------------**/
};