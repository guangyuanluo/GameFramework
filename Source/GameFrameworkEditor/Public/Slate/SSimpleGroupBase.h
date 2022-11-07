// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/SCompoundWidget.h"
#include "EditorCommon.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"

/**
 * 
 */
class GAMEFRAMEWORKEDITOR_API SSimpleGroupBase : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSimpleGroupBase)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList);

    virtual UDataTable* GetGroupDataTable() = 0;
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetGroupItemSource() = 0;
    virtual TArray<TSharedPtr<FConfigTableRowWrapper>> GetGroupSource() = 0;
    virtual FName NewRowInit(FName Name) = 0;

protected:
	/**组相关变量**/
	UDataTable* mDataTable;
	TSharedPtr<SListView<TSharedPtr<FConfigTableRowWrapper>>> mGroupListView;
	TArray<TSharedPtr<FConfigTableRowWrapper>> mGroupSource;
	TSharedPtr<class SGameDataTableRowEditor> mGroupEditorRow;
	FName mGroupHighlightedRowName;
	/**组相关变量**/

	/**-----------------组界面相关---------------------**/
	void ConstructGroupWidgetSources();
	//总界面
	TSharedRef<SWidget> ConstructGroupPage();
	TSharedRef<SWidget> ConstructGroupRowEditor();
	FName GroupTableRowAdded(FName Name);
	virtual bool GroupTableRowPreRemove(FName Name);
	void GroupTableRowRemoved(FName Name);
	void GroupTableRowModified(FName Name);
	void GroupTableSetHighlightedRow(FName Name);
	void OnGroupSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo);
	TSharedRef<ITableRow> GroupListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable);
	/**-----------------组界面相关---------------------**/
};