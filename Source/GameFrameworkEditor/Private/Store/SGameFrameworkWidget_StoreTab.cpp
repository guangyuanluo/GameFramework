// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_StoreTab.h"
#include "Modules/Store/StoreSetting.h"
#include "Modules/Store/StoreConfigTableRow.h"
#include "Modules/Store/GoodsConfigTableRow.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "DataTableEditorUtils.h"

UDataTable* SGameFrameworkWidget_StoreTab::GetGroupDataTable() {
    const UStoreSetting* StoreSetting = GetDefault<UStoreSetting>();
    return StoreSetting->StoreTable.LoadSynchronous();
}
TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_StoreTab::GetGroupItemSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UStoreSetting* StoreSetting = GetDefault<UStoreSetting>();
    auto DataTable = StoreSetting->GoodsTable.LoadSynchronous();
    if (DataTable != nullptr) {
        auto TableUsingStruct = DataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FGoodsConfigTableRow*> DataArr;
        DataTable->GetAllRows("", DataArr);
        for (auto Index = 0; Index < DataArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, DataArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_StoreTab::GetGroupSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UStoreSetting* StoreSetting = GetDefault<UStoreSetting>();
    auto DataTable = StoreSetting->StoreTable.LoadSynchronous();
    if (DataTable != nullptr) {
        auto TableUsingStruct = DataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FStoreConfigTableRow*> DataArr;
        DataTable->GetAllRows("", DataArr);
        for (auto Index = 0; Index < DataArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, DataArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

FName SGameFrameworkWidget_StoreTab::NewRowInit(FName Name) {
    auto DataTable = GetGroupDataTable();
    if (DataTable) {
        int32 NewId = ApplyId();
        FName RowName = *FString::FromInt(NewId);
        if (FDataTableEditorUtils::RenameRow(DataTable, Name, RowName)) {
            FStoreConfigTableRow* FindRow = DataTable->FindRow<FStoreConfigTableRow>(RowName, "");
            FindRow->StoreID = NewId;
            return RowName;
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_StoreTab::ApplyId() {
    auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
    auto IdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("StoreIdSeed", "");
    if (IdSeed == nullptr) {
        FDataTableSeedConfigTableRow NewRow;
        NewRow.SeedKey = "StoreIdSeed";
        NewRow.SeedValue = 0;
        SeedTable->AddRow("StoreIdSeed", NewRow);
        IdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("StoreIdSeed", "");
    }
    int Id = ++IdSeed->SeedValue;

    SeedTable->MarkPackageDirty();
    return Id;
}

bool SGameFrameworkWidget_StoreTab::GroupTableRowPreRemove(FName Name) {
    return true;
}