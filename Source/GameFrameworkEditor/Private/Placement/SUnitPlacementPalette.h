// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SUnitPlacementPalette : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SUnitPlacementPalette){}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct( const FArguments& InArgs );

    void BuildList();
    TSharedRef<ITableRow> MakeListViewWidget(TSharedPtr<struct FUnitPlacementListItem> MediaPlacement, const TSharedRef<STableViewBase>& OwnerTable);
    void OnSelectionChanged(TSharedPtr<struct FUnitPlacementListItem> UnitPlacement, ESelectInfo::Type SelectionType);
    FReply OnDraggingListViewWidget(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

private:
    TSharedPtr<struct FUnitPlacementListItem> SelectedUnitPlacement;
    TArray<TSharedPtr<struct FUnitPlacementListItem>> PlacementList;
};
