// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_GoodsTab.h"
#include "Modules/Store/StoreSetting.h"
#include "Modules/Store/GoodsConfigTableRow.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "DataTableEditorUtils.h"

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_GoodsTab::GetRowSource() {
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

class UDataTable* SGameFrameworkWidget_GoodsTab::GetDataTable() {
    const UStoreSetting* StoreSetting = GetDefault<UStoreSetting>();
    return StoreSetting->GoodsTable.LoadSynchronous();
}

FName SGameFrameworkWidget_GoodsTab::NewRowInit(FName Name) {
    auto MoneyTypeDataTable = GetDataTable();
    if (MoneyTypeDataTable) {
        int32 NewId = ApplyId();
        FName RowName = *FString::FromInt(NewId);
        if (FDataTableEditorUtils::RenameRow(MoneyTypeDataTable, Name, RowName)) {
            FGoodsConfigTableRow* FindRow = MoneyTypeDataTable->FindRow<FGoodsConfigTableRow>(RowName, "");
            FindRow->GoodsID = NewId;
            return RowName;
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_GoodsTab::ApplyId() {
    auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
    auto IdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("GoodsIdSeed", "");
    if (IdSeed == nullptr) {
        FDataTableSeedConfigTableRow NewRow;
        NewRow.SeedKey = "GoodsIdSeed";
        NewRow.SeedValue = 0;
        SeedTable->AddRow("GoodsIdSeed", NewRow);
        IdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("GoodsIdSeed", "");
    }
    int Id = ++IdSeed->SeedValue;

    SeedTable->MarkPackageDirty();
    return Id;
}

bool SGameFrameworkWidget_GoodsTab::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    return true;
}