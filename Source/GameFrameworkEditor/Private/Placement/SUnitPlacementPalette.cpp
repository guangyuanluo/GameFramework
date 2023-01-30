// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SUnitPlacementPalette.h"
#include "Widgets/Layout/SScrollBorder.h"
#include "LevelEditor.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "Character/CoreCharacter.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"

#if ENGINE_MAJOR_VERSION > 4
#include "ThumbnailRendering/ThumbnailManager.h"
#endif

#define LOCTEXT_NAMESPACE "SUnitPlacementPalette"

/** The asset of the asset view */
struct FUnitPlacementListItem {
    FUnitPlacementListItem() = default;

    bool IsValid() const {
        return UnitBundle.IsUAsset();
    }

    FText DisplayName;
    FAssetData UnitBundle;
};

/** The list view mode of the asset view */
class SUnitPlacementListView : public SListView<TSharedPtr<FUnitPlacementListItem>> {
public:
    virtual bool SupportsKeyboardFocus() const override {
        return false;
    }
    virtual FReply OnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override {
        return FReply::Unhandled();
    }
};

void SUnitPlacementPalette::Construct( const FArguments& InArgs )
{
    BuildList();

    TSharedRef<SUnitPlacementListView> ListViewWidget =
        SNew(SUnitPlacementListView)
        .SelectionMode(ESelectionMode::Single)
        .ListItemsSource(&PlacementList)
        .OnGenerateRow(this, &SUnitPlacementPalette::MakeListViewWidget)
        .OnSelectionChanged(this, &SUnitPlacementPalette::OnSelectionChanged)
        .ItemHeight(35);

    ChildSlot
        [
            SNew(SScrollBorder, ListViewWidget)
            [
                ListViewWidget
            ]
        ];
}

TSharedRef<ITableRow> SUnitPlacementPalette::MakeListViewWidget(TSharedPtr<FUnitPlacementListItem> UnitPlacement, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!UnitPlacement.IsValid() || !UnitPlacement->IsValid())
	{
		return SNew(STableRow<TSharedPtr<FUnitPlacementListItem>>, OwnerTable);
	}

	TSharedRef< STableRow<TSharedPtr<FUnitPlacementListItem>> > TableRowWidget =
		SNew(STableRow<TSharedPtr<FUnitPlacementListItem>>, OwnerTable)
#if ENGINE_MAJOR_VERSION > 4
		.Style(FAppStyle::Get(), "ContentBrowser.AssetListView.TableRow")
#else
		.Style(FEditorStyle::Get(), "ContentBrowser.AssetListView.TableRow")
#endif
		.OnDragDetected(this, &SUnitPlacementPalette::OnDraggingListViewWidget);

	// Get the MediaSource thumbnail or the MediaBundle is not loaded
#if ENGINE_MAJOR_VERSION > 4
	TSharedPtr<FAssetThumbnailPool> ThumbnailPool = UThumbnailManager::Get().GetSharedThumbnailPool();
#else
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FAssetThumbnailPool> ThumbnailPool = LevelEditorModule.GetFirstLevelEditor()->GetThumbnailPool();
#endif

	FAssetData ThumbnailAssetData = UnitPlacement->UnitBundle;
	if (UnitPlacement->UnitBundle.IsAssetLoaded())
	{
        ACoreCharacter* CharacterBundle = Cast<ACoreCharacter>(UnitPlacement->UnitBundle.GetAsset());
		if (CharacterBundle)
		{
			ThumbnailAssetData = FAssetData(CharacterBundle);
		}
	}
	TSharedPtr< FAssetThumbnail > Thumbnail = MakeShareable(new FAssetThumbnail(ThumbnailAssetData, 32, 32, ThumbnailPool));

	// Create the TableRow content
	TSharedRef<SWidget> Content =
		SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("NoBorder"))
		.Padding(0)
		.Cursor(EMouseCursor::GrabHand)
		[
			SNew(SHorizontalBox)
			// Icon
			+ SHorizontalBox::Slot()
			.Padding(0)
			.AutoWidth()
			[
				SNew(SBorder)
				.Padding(4.0f)
#if ENGINE_MAJOR_VERSION > 4
				.BorderImage(FAppStyle::GetBrush("ContentBrowser.ThumbnailShadow"))
#else
				.BorderImage(FEditorStyle::GetBrush("ContentBrowser.ThumbnailShadow"))
#endif
				[
					SNew(SBox)
					.WidthOverride(35.0f)
					.HeightOverride(35.0f)
					[
						SNew(SBorder)
#if ENGINE_MAJOR_VERSION > 4
						.BorderImage(FAppStyle::GetBrush("ContentBrowser.ThumbnailShadow"))
#else
						.BorderImage(FEditorStyle::GetBrush("ContentBrowser.ThumbnailShadow"))						
#endif
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							Thumbnail->MakeThumbnailWidget()
						]
					]
				]
			]

			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.Padding(2, 0, 4, 0)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(0, 0, 0, 1)
				.AutoHeight()
				[
					SNew(STextBlock)
#if ENGINE_MAJOR_VERSION > 4
					.TextStyle(FAppStyle::Get(), "PlacementBrowser.Asset.Name")
#else
					.TextStyle(FEditorStyle::Get(), "PlacementBrowser.Asset.Name")
#endif
					.Text(UnitPlacement->DisplayName)
				]
			]
		];

	TableRowWidget->SetContent(Content);

	return TableRowWidget;

}

void SUnitPlacementPalette::OnSelectionChanged(TSharedPtr<FUnitPlacementListItem> UnitPlacement, ESelectInfo::Type SelectionType)
{
	SelectedUnitPlacement = UnitPlacement;
}

FReply SUnitPlacementPalette::OnDraggingListViewWidget(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (SelectedUnitPlacement.IsValid())
		{
			// We have an active brush builder, start a drag-drop
			TArray<FAssetData> InAssetData;
			InAssetData.Add(SelectedUnitPlacement->UnitBundle);
			return FReply::Handled().BeginDragDrop(FAssetDragDropOp::New(InAssetData));
		}
	}

	return FReply::Unhandled();
}

void SUnitPlacementPalette::BuildList()
{
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

    auto UnitInfoSource = GameFrameworkEditorModule.GetEditorWidgetTool()->GetUnitInfoSource();

    for (int Index = 0; Index < UnitInfoSource.Num(); ++Index) {
        FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoSource[Index]->ConfigTableRow);
        if (RowData && !RowData->CharacterClass.IsNull()) {
            TSharedPtr<FUnitPlacementListItem> NewItem = MakeShared<FUnitPlacementListItem>();
            NewItem->DisplayName = FText::FromString(FString::Format(TEXT("{0}.{1}"), { RowData->UnitId, RowData->UnitName }));
            NewItem->UnitBundle = FAssetData(RowData->CharacterClass.LoadSynchronous());
            PlacementList.Add(NewItem);
        }
    }
}

#undef LOCTEXT_NAMESPACE