// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_SkillGroupTab.h"
#include "Modules/Skill/SkillGroupConfigTableRow.h"
#include "Modules/Skill/SkillSetting.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "DataTableEditorUtils.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_SkillGroupTab::GetRowSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetSkillGroupSource();
}

class UDataTable* SGameFrameworkWidget_SkillGroupTab::GetDataTable() {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    return SkillSetting->SkillGroupTable.LoadSynchronous();
}

FName SGameFrameworkWidget_SkillGroupTab::NewRowInit(FName Name) {
    auto SkillDataTable = GetDataTable();
    if (SkillDataTable) {
        int32 NewId = ApplySkillGroupId();
        FName RowName = *FString::FromInt(NewId);
        if (FDataTableEditorUtils::RenameRow(SkillDataTable, Name, RowName)) {
            FSkillGroupConfigTableRow* FindRow = SkillDataTable->FindRow<FSkillGroupConfigTableRow>(RowName, "");
            FindRow->SkillGroupID = NewId;
            FindRow->Description = TEXT("这里添加技能模组描述");
            return RowName;
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_SkillGroupTab::ApplySkillGroupId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto SkillIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("SkillGroupIdSeed", "");
	if (SkillIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "SkillGroupIdSeed";
		NewRow.SeedValue = 1;
		SeedTable->AddRow("SkillGroupIdSeed", NewRow);
		SkillIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("SkillGroupIdSeed", "");
	}
    int SkillId = ++SkillIdSeed->SeedValue;
    SeedTable->MarkPackageDirty();

    return SkillId;
}

bool SGameFrameworkWidget_SkillGroupTab::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    FString UseInfo;
    FSkillGroupConfigTableRow* RowData = (FSkillGroupConfigTableRow*)RemoveRow;
    if (GameFrameworkEditorModule.GetEditorWidgetTool()->IsSkillGroupIdUse(RowData->SkillGroupID, UseInfo)) {
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(UseInfo), 5.0);
        return false;
    }
    return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION