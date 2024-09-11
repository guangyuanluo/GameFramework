// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_Exp.h"
#include "Modules/Exp/ExpSetting.h"
#include "Modules/Exp/ExpTypeConfigTableRow.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "DataTableEditorUtils.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_Exp::GetRowSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetExpTypeSource();
}

class UDataTable* SGameFrameworkWidget_Exp::GetDataTable() {
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    return ExpSetting->ExpTypeTable.LoadSynchronous();
}

FName SGameFrameworkWidget_Exp::NewRowInit(FName Name) {
    auto ExpTypeDataTable = GetDataTable();
    if (ExpTypeDataTable) {
        int32 NewId = ApplyExpTypeId();
        if (NewId != -1) {
            FName RowName = *FString::FromInt(NewId);
            if (FDataTableEditorUtils::RenameRow(ExpTypeDataTable, Name, RowName)) {
                FExpTypeConfigTableRow* FindRow = ExpTypeDataTable->FindRow<FExpTypeConfigTableRow>(RowName, "");
                FindRow->ExpTypeId = NewId;
                return RowName;
            }
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_Exp::ApplyExpTypeId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto ExpTypeIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("ExpTypeIdSeed", "");
	if (ExpTypeIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "ExpTypeIdSeed";
		NewRow.SeedValue = 1;
		SeedTable->AddRow("ExpTypeIdSeed", NewRow);
		ExpTypeIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("ExpTypeIdSeed", "");
	}
	int ExpTypeId = ++ExpTypeIdSeed->SeedValue;
	SeedTable->MarkPackageDirty();
	return ExpTypeId;
}

void SGameFrameworkWidget_Exp::ConstructWidgetSources() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();

    auto TableUsingStruct = ExpTypeDataTable->GetRowStruct();
    int32 StructureSize = TableUsingStruct->GetStructureSize();
    {
        FExpTypeConfigTableRow* FindRow = (FExpTypeConfigTableRow*)ExpTypeDataTable->FindRowUnchecked(*FString::FromInt((int)EExpTypeEnum::ExpType_Role));
        if (FindRow == nullptr) {
            //增加默认的角色升级经验类型
            FExpTypeConfigTableRow* NewRawRowData = (FExpTypeConfigTableRow*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);

            NewRawRowData->ExpTypeId = 0;
            NewRawRowData->ExpType = EExpTypeEnum::ExpType_Role;

            ExpTypeDataTable->AddRow(*FString::FromInt(NewRawRowData->ExpTypeId), *NewRawRowData);

            ExpTypeDataTable->MarkPackageDirty();
        }
    }

    TSet<int32> InvalidExpTypeId;
    ExpTypeDataTable->ForeachRow<FExpTypeConfigTableRow>("", [this, &InvalidExpTypeId](const FName& key, const FExpTypeConfigTableRow& value) -> void {
        UDataTable* DataTable = LoadObject<UDataTable>(NULL, *value.ExpLevelTable.GetUniqueID().GetAssetPathString(), NULL, LOAD_None, NULL);
        if (DataTable == nullptr) {
            InvalidExpTypeId.Add(value.ExpTypeId);
        }
    });

    if (InvalidExpTypeId.Num() > 0) {
        for (auto Iter = InvalidExpTypeId.CreateIterator(); Iter; ++Iter) {
            FExpTypeConfigTableRow* FindRow = ExpTypeDataTable->FindRow<FExpTypeConfigTableRow>(*FString::FromInt(*Iter), "");
        }
        ExpTypeDataTable->MarkPackageDirty();
    }

    SSimpleRow::ConstructWidgetSources();
}

bool SGameFrameworkWidget_Exp::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    return true;
}

bool SGameFrameworkWidget_Exp::AllRowModified(FName Name, struct FConfigTableRowBase* ChangeRow, struct FConfigTableRowBase* OriginRow) {
    return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION