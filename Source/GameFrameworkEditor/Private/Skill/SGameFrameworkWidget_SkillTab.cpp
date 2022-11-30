// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_SkillTab.h"
#include "Modules/Skill/SkillConfigTableRow.h"
#include "Modules/Skill/SkillSetting.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "DataTableEditorUtils.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_SkillTab::GetRowSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetSkillSource();
}

class UDataTable* SGameFrameworkWidget_SkillTab::GetDataTable() {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    return SkillSetting->SkillTable.LoadSynchronous();
}

FName SGameFrameworkWidget_SkillTab::NewRowInit(FName Name) {
    auto SkillDataTable = GetDataTable();
    if (SkillDataTable) {
        int32 NewId = ApplySkillId();
        FName RowName = *FString::FromInt(NewId);
        if (FDataTableEditorUtils::RenameRow(SkillDataTable, Name, RowName)) {
            FSkillConfigTableRow* FindRow = SkillDataTable->FindRow<FSkillConfigTableRow>(RowName, "");
            FindRow->SkillId = NewId;
            FindRow->Description = TEXT("这里添加技能描述");
            return RowName;
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_SkillTab::ApplySkillId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto SkillIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("SkillIdSeed", "");
	if (SkillIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "SkillIdSeed";
		NewRow.SeedValue = 1;
		SeedTable->AddRow("SkillIdSeed", NewRow);
		SkillIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("SkillIdSeed", "");
	}
    int SkillId = ++SkillIdSeed->SeedValue;
    SeedTable->MarkPackageDirty();

    return SkillId;
}

bool SGameFrameworkWidget_SkillTab::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION