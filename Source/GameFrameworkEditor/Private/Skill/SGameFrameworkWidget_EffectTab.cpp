// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_EffectTab.h"
#include "Modules/Skill/EffectConfigTableRow.h"
#include "Modules/Skill/SkillSetting.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "DataTableEditorUtils.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_EffectTab::GetRowSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetEffectSource();
}

class UDataTable* SGameFrameworkWidget_EffectTab::GetDataTable() {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    return SkillSetting->EffectTable.LoadSynchronous();
}

FName SGameFrameworkWidget_EffectTab::NewRowInit(FName Name) {
    auto EffectDataTable = GetDataTable();
    if (EffectDataTable) {
        int32 NewId = ApplyEffectId();
        FName RowName = *FString::FromInt(NewId);
        if (FDataTableEditorUtils::RenameRow(EffectDataTable, Name, RowName)) {
            FEffectConfigTableRow* FindRow = EffectDataTable->FindRow<FEffectConfigTableRow>(RowName, "");
            FindRow->EffectID = NewId;
            FindRow->Description = TEXT("这里添加效果描述");
            return RowName;
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_EffectTab::ApplyEffectId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto SkillIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("EffectIdSeed", "");
	if (SkillIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "EffectIdSeed";
		NewRow.SeedValue = 1;
		SeedTable->AddRow("EffectIdSeed", NewRow);
		SkillIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("EffectIdSeed", "");
	}
    int SkillId = ++SkillIdSeed->SeedValue;
    SeedTable->MarkPackageDirty();

    return SkillId;
}

bool SGameFrameworkWidget_EffectTab::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION