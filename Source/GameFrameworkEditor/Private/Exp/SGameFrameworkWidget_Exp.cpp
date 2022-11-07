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
                FindRow->ExpTypeDescription = TEXT("这里添加经验描述");
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

	TSet<int> ExpTypeIdSet;
	ExpTypeIdSet.Add(FExpTypeConfigTableRow::RoleExpType);
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();
	if (ExpTypeDataTable != nullptr) {
		TArray<FExpTypeConfigTableRow*> ExpTypeArr;
        ExpTypeDataTable->GetAllRows("", ExpTypeArr);
		for (auto Index = 0; Index < ExpTypeArr.Num(); ++Index) {
			ExpTypeIdSet.Add(ExpTypeArr[Index]->ExpTypeId);
		}
	}
	int ExpTypeId = -1;
	if (ExpTypeIdSet.Num() >= 255) {
		
	}
	else {
		do {
			ExpTypeId = (++ExpTypeIdSeed->SeedValue) % 255;
		} while (ExpTypeIdSet.Contains(ExpTypeId));
	}
	
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
        FExpTypeConfigTableRow* FindRow = (FExpTypeConfigTableRow*)ExpTypeDataTable->FindRowUnchecked(*FString::FromInt(FExpTypeConfigTableRow::RoleExpType));
        if (FindRow == nullptr) {
            //增加默认的角色升级经验类型
            FExpTypeConfigTableRow* NewRawRowData = (FExpTypeConfigTableRow*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);

            NewRawRowData->ExpTypeId = FExpTypeConfigTableRow::RoleExpType;
            NewRawRowData->ExpTypeDescription = TEXT("角色升级经验");

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
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    FString UseInfo;
    FExpTypeConfigTableRow* RowData = (FExpTypeConfigTableRow*)RemoveRow;
    if (GameFrameworkEditorModule.GetEditorWidgetTool()->IsExpTypeUse(RowData->ExpTypeId, UseInfo)) {
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(UseInfo), 5.0);
        return false;
    }
    return true;
}

bool SGameFrameworkWidget_Exp::AllRowModified(FName Name, struct FConfigTableRowBase* ChangeRow, struct FConfigTableRowBase* OriginRow) {
    if (OriginRow->GetUniqueId() == FExpTypeConfigTableRow::RoleExpType) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("默认经验类型不能修改")), 5.0);
        return false;
    }
    return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION