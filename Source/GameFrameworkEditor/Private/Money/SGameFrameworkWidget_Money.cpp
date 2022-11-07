// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_Money.h"
#include "Modules/Money/MoneySetting.h"
#include "Modules/Money/MoneyTypeConfigTableRow.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "DataTableEditorUtils.h"

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_Money::GetRowSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetMoneyTypeSource();
}

class UDataTable* SGameFrameworkWidget_Money::GetDataTable() {
    const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
    return MoneySetting->MoneyTypeTable.LoadSynchronous();
}

FName SGameFrameworkWidget_Money::NewRowInit(FName Name) {
    auto MoneyTypeDataTable = GetDataTable();
    if (MoneyTypeDataTable) {
        int32 NewId = ApplyMoneyTypeId();
        if (NewId != -1) {
            FName RowName = *FString::FromInt(NewId);
            if (FDataTableEditorUtils::RenameRow(MoneyTypeDataTable, Name, RowName)) {
                FMoneyTypeConfigTableRow* FindRow = MoneyTypeDataTable->FindRow<FMoneyTypeConfigTableRow>(RowName, "");
                FindRow->MoneyTypeId = NewId;
                FindRow->MoneyTypeDescription = TEXT("这里添加货币描述");
                return RowName;
            }
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_Money::ApplyMoneyTypeId() {
    auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
    auto moneyTypeIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("MoneyTypeIdSeed", "");
    if (moneyTypeIdSeed == nullptr) {
        FDataTableSeedConfigTableRow NewRow;
        NewRow.SeedKey = "MoneyTypeIdSeed";
        NewRow.SeedValue = 0;
        SeedTable->AddRow("MoneyTypeIdSeed", NewRow);
        moneyTypeIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("MoneyTypeIdSeed", "");
    }
    TSet<int> MoneyTypeIdSet;
    const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
    auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();
    if (MoneyTypeDataTable != nullptr) {
        TArray<FMoneyTypeConfigTableRow*> MoneyTypeArr;
        MoneyTypeDataTable->GetAllRows("", MoneyTypeArr);
        for (auto Index = 0; Index < MoneyTypeArr.Num(); ++Index) {
            MoneyTypeIdSet.Add(MoneyTypeArr[Index]->MoneyTypeId);
        }
    }
    int moneyTypeId = -1;
    if (MoneyTypeIdSet.Num() >= 255) {

    }
    else {
        do {
            moneyTypeId = (++moneyTypeIdSeed->SeedValue) % 255;
        } while (MoneyTypeIdSet.Contains(moneyTypeId));
    }

    SeedTable->MarkPackageDirty();
    return moneyTypeId;
}

bool SGameFrameworkWidget_Money::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    FString UseInfo;
    FMoneyTypeConfigTableRow* RowData = (FMoneyTypeConfigTableRow*)RemoveRow;
    if (GameFrameworkEditorModule.GetEditorWidgetTool()->IsMoneyTypeUse(RowData->MoneyTypeId, UseInfo)) {
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(UseInfo), 5.0);
        return false;
    }
    return true;
}