// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_UnitGroup.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"
#include "Modules/Unit/UnitGroupConfigTableRow.h"
#include "Modules/Unit/UnitSetting.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "DataTableEditorUtils.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

UDataTable* SGameFrameworkWidget_UnitGroup::GetGroupDataTable() {
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitGroupDataTable = UnitSetting->UnitGroupTable.LoadSynchronous();
    return UnitGroupDataTable;
}
TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_UnitGroup::GetGroupItemSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetUnitInfoSource();
}

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_UnitGroup::GetGroupSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetUnitGroupSource();
}

FName SGameFrameworkWidget_UnitGroup::NewRowInit(FName Name) {
    auto GroupDataTable = GetGroupDataTable();
    if (GroupDataTable) {
        int32 NewUnitGroupId = ApplyUnitGroupId();
        FName RowName = *FString::FromInt(NewUnitGroupId);
        if (FDataTableEditorUtils::RenameRow(GroupDataTable, Name, RowName)) {
            FUnitGroupConfigTableRow* FindRow = GroupDataTable->FindRow<FUnitGroupConfigTableRow>(RowName, "");
            FindRow->GroupId = NewUnitGroupId;
            FindRow->Description = TEXT("这里添加单位组描述");
            return RowName;
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_UnitGroup::ApplyUnitGroupId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto UnitGroupIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("UnitGroupIdSeed", "");
	if (UnitGroupIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "UnitGroupIdSeed";
		NewRow.SeedValue = 1;
		SeedTable->AddRow("UnitGroupIdSeed", NewRow);
		UnitGroupIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("UnitGroupIdSeed", "");
	}

	int UnitGroupId = UnitGroupIdSeed->SeedValue++;
	
	SeedTable->MarkPackageDirty();
	return UnitGroupId;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION