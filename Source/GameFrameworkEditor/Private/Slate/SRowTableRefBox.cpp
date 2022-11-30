// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SRowTableRefBox.h"

#include "Engine/DataTable.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SComboBox.h"

#define LOCTEXT_NAMESPACE "SRowTableRefBox"

int SearchBoxId = -1000;
int NoneId = -1;

void SRowTableRefBox::Construct( const FArguments& InArgs, UDataTable* InRefDataTable, int InCurrentSelectId)
{
    RefDataTable = InRefDataTable;
    CurrentSelectId = InCurrentSelectId;

    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SAssignNew(RowComboBox, SComboBox<TSharedPtr<int>>)
            .OptionsSource(&CachedRowNames)
            .OnSelectionChanged(this, &SRowTableRefBox::OnSelectionChanged)
            .OnGenerateWidget(this, &SRowTableRefBox::OnGenerateWidget)
            .Content()
            [
                SNew(STextBlock).Text(this, &SRowTableRefBox::GetCurrentNameAsText)
            ]
        ]
    ];

    OnSearchTextChanged(FText::GetEmpty());
}

void SRowTableRefBox::SetSelect(int InCurrentSelectId) {
    CurrentSelectId = InCurrentSelectId;

    SearchText = FText::GetEmpty();
    if (SearchBox) {
        SearchBox->SetText(SearchText);
    }

    CachedRowNames.Empty();
    CachedRowNames.Add(MakeShareable(new int(SearchBoxId)));
    CachedRowNames.Add(MakeShareable(new int(NoneId)));

    TSharedPtr<int> FindSelectItem;

    const TMap<FName, uint8*>& RowMap = RefDataTable->GetRowMap();
    for (auto Iter = RowMap.CreateConstIterator(); Iter; ++Iter) {
        FConfigTableRowBase* TableRow = (FConfigTableRowBase*)(Iter->Value);

        if (SearchText.IsEmpty() || TableRow->GetSimpleDescription().Find(SearchText.ToString()) >= 0) {
            TSharedPtr<int> NewRowName(new int);
            *NewRowName = TableRow->GetUniqueId();

            if (*NewRowName == InCurrentSelectId) {
                FindSelectItem = NewRowName;
            }
            CachedRowNames.Add(NewRowName);
        }
    }

    if (FindSelectItem) {
        RowComboBox->SetSelectedItem(FindSelectItem);
    }
    else {
        RowComboBox->ClearSelection();
    }

    if (RowSelectChanged.IsBound()) {
        RowSelectChanged.Execute(CurrentSelectId);
    }
}

TSharedRef<SWidget> SRowTableRefBox::OnGenerateWidget(TSharedPtr<int> InItem) {
    if (!InItem.IsValid()) {
        return SNew(STextBlock).Text(FText::FromName(NAME_None));
    }
    else if (*InItem == SearchBoxId) {
        if (!SearchBox) {
            SAssignNew(SearchBox, SSearchBox)
                .HintText(LOCTEXT("SearchHint", "Search..."))
                .OnTextChanged(this, &SRowTableRefBox::OnSearchTextChanged)
                .OnTextCommitted(this, &SRowTableRefBox::OnSearchTextCommitted)
                .DelayChangeNotificationsWhileTyping(true);
        }

        return SearchBox.ToSharedRef();
    }
    else if (*InItem == NoneId) {
        return SNew(STextBlock).Text(FText::FromString(TEXT("None")));
    }
    else {
        return SNew(STextBlock).Text(FText::FromString(GetRowDisplayName(*InItem)));
    }
}

void SRowTableRefBox::OnSelectionChanged(TSharedPtr<int> InItem, ESelectInfo::Type InSeletionInfo) {
    if (InItem.IsValid() && *InItem != SearchBoxId && *InItem != CurrentSelectId) {
        CurrentSelectId = *InItem;

        SearchText = FText::GetEmpty();
        SearchBox->SetText(SearchText);

        if (RowSelectChanged.IsBound()) {
            RowSelectChanged.Execute(CurrentSelectId);
        }
    }
}

FText SRowTableRefBox::GetCurrentNameAsText() const {
    return FText::FromString(GetRowDisplayName(CurrentSelectId));
}

FString SRowTableRefBox::GetRowDisplayName(int Id) const {
    const TMap<FName, uint8*>& RowMap = RefDataTable->GetRowMap();
    for (auto Iter = RowMap.CreateConstIterator(); Iter; ++Iter) {
        FConfigTableRowBase* TableRow = (FConfigTableRowBase*)(Iter->Value);
        if (TableRow->GetUniqueId() == Id) {
            return TableRow->GetSimpleDescription();
        }
    }
    return TEXT("");
}

void SRowTableRefBox::OnSearchTextChanged(const FText& InFilterText) {
    SearchText = InFilterText;
    
    CachedRowNames.Empty();
    CachedRowNames.Add(MakeShareable(new int(SearchBoxId)));
    if (SearchText.IsEmpty()) {
        CachedRowNames.Add(MakeShareable(new int(NoneId)));
    }

    const TMap<FName, uint8*>& RowMap = RefDataTable->GetRowMap();
    for (auto Iter = RowMap.CreateConstIterator(); Iter; ++Iter) {
        FConfigTableRowBase* TableRow = (FConfigTableRowBase*)(Iter->Value);

        if (SearchText.IsEmpty() || TableRow->GetSimpleDescription().Find(SearchText.ToString()) >= 0) {
            int* NewRowName = new int();
            *NewRowName = TableRow->GetUniqueId();
            CachedRowNames.Add(MakeShareable(NewRowName));
        }
    }
    RowComboBox->RefreshOptions();
}

void SRowTableRefBox::OnSearchTextCommitted(const FText& InFilterText, ETextCommit::Type CommitType) {
    if (!InFilterText.EqualTo(SearchText)) {
        OnSearchTextChanged(InFilterText);
    }
}

#undef LOCTEXT_NAMESPACE
