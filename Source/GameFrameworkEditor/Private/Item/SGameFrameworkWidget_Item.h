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
class SGameFrameworkWidget_Item : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameFrameworkWidget_Item)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList);
	virtual ~SGameFrameworkWidget_Item() override;

private:
	/**物品相关变量**/
	int mItemTabIndex;

	TSharedPtr<SListView<TSharedPtr<FConfigTableRowWrapper>>> mItemTypeListView;
	TArray<TSharedPtr<FConfigTableRowWrapper>> mItemTypeSource;
	TSharedPtr<class SGameDataTableRowEditor> mItemTypeEditorRow;
	FName mItemTypeHighlightedRowName;

	TSharedPtr<SListView<TSharedPtr<FConfigTableRowWrapper>>> mItemInfoListView;
	TArray<TSharedPtr<FConfigTableRowWrapper>> mItemInfoSource;
	TSharedPtr<class IStructureDetailsView> ItemInfoStructureDetailsView;
	TSharedPtr<class SGameDataTableRowEditor> mItemInfoEditorRow;
	FName mItemInfoHighlightedRowName;
	/**物品相关变量**/

	/**-----------------物品界面相关---------------------**/
	void ConstructItemWidgetSources();
	int32 GetItemTabIndex() const;
	TSharedRef<SWidget> ConstructItemPageButton(const FString& PageName, FOnClicked ClickCallback);
	FReply ItemPackageTabClicked();
	FReply ItemItemTypeTabClicked();
	FReply ItemItemInfoTabClicked();
	//物品总界面
	TSharedRef<SWidget> ConstructItemPage();
	//物品类型界面
	TSharedRef<SWidget> ConstructItemTypePage();
	TSharedRef<SWidget> ConstructItemTypeRowEditor();
	bool ItemTypeTableRowPreAdd(FName Name);
	FName ItemTypeTableRowAdded(FName Name);
	bool ItemTypeTableRowPreRemove(FName Name);
	void ItemTypeTableRowRemoved(FName Name);
	void ItemTypeTableRowModified(FName Name);
	void ItemTypeTableSetHighlightedRow(FName Name);
	void OnItemTypeSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo);
	TSharedRef<ITableRow> ItemTypeListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable);
	int32 ApplyItemTypeId();
	//物品信息界面
	TSharedRef<SWidget> ConstructItemInfoPage();
	TSharedRef<SWidget> ConstructItemInfoRowEditor();
	bool ItemInfoTableRowPreAdd(FName Name);
	FName ItemInfoTableRowAdded(FName Name);
	bool ItemInfoTableRowPreRemove(FName Name);
	void ItemInfoTableRowRemoved(FName Name);
	void ItemInfoTableRowModified(FName Name);
	void ItemInfoTableSetHighlightedRow(FName Name);
	void OnItemInfoSelectionChanged(TSharedPtr<FConfigTableRowWrapper> InNewSelection, ESelectInfo::Type InSelectInfo);
	TSharedRef<ITableRow> ItemInfoListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable);
	int32 ApplyItemId();
	/**-----------------物品界面相关---------------------**/
};