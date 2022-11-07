// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Framework/SlateDelegates.h"

class FArrangedChildren;

DECLARE_DELEGATE_OneParam(FOnRowSelectChanged, int);

/**
 * Vector Slate control
 */
class GAMEFRAMEWORKEDITOR_API SRowTableRefBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRowTableRefBox)
		{}

    SLATE_END_ARGS()


    FOnRowSelectChanged RowSelectChanged;

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct( const FArguments& InArgs, UDataTable* InRefDataTable, int InCurrentSelectId);

    void SetSelect(int InCurrentSelectId);

private:
    TSoftObjectPtr<UDataTable> RefDataTable; // weak obj ptr couldn't handle reimporting
    int CurrentSelectId;

    TArray<TSharedPtr<int>> CachedRowNames;
    TSharedPtr<class SSearchBox> SearchBox;
    TSharedPtr<class SComboBox<TSharedPtr<int>>> RowComboBox;

    FText SearchText;

    TSharedRef<SWidget> OnGenerateWidget(TSharedPtr<int> InItem);
    void OnSelectionChanged(TSharedPtr<int> InItem, ESelectInfo::Type InSeletionInfo);
    FText GetCurrentNameAsText() const;
    FString GetRowDisplayName(int Id) const;

    void OnSearchTextChanged(const FText& InFilterText);
    void OnSearchTextCommitted(const FText& InFilterText, ETextCommit::Type CommitType);
    void UpdateFilter();
};
