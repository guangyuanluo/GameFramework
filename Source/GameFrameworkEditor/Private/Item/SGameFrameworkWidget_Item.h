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
	/**-----------------物品界面相关---------------------**/
};