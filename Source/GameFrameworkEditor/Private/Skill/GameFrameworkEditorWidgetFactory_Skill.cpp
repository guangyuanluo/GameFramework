#include "GameFrameworkEditorWidgetFactory_Skill.h"
#include "SGameFrameworkWidget_SkillMain.h"
#include "Modules/Skill/SkillSetting.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"

FText GameFrameworkEditorWidgetFactory_Skill::GetDisplayName() {
	return FText::FromString(TEXT("技能"));
}

FText GameFrameworkEditorWidgetFactory_Skill::GetDisplayToolTip() {
	return FText::FromString(TEXT("编辑游戏中的技能"));
}

FText GameFrameworkEditorWidgetFactory_Skill::GetWindowName() {
	return FText::FromString(TEXT("技能编辑器"));
}

TSharedRef<SWidget> GameFrameworkEditorWidgetFactory_Skill::ConstructPage(TSharedPtr<FUICommandList> CommandList) {
	return SNew(SGameFrameworkWidget_SkillMain, CommandList);
}

void GameFrameworkEditorWidgetFactory_Skill::CheckEditorTableNoExistAndCreate() {
	
}

void GameFrameworkEditorWidgetFactory_Skill::Export() {

}

bool GameFrameworkEditorWidgetFactory_Skill::CheckOpenCondition() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();

    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    if (SkillDataTable == nullptr) {
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置技能表，请前往设置里配置")), 5.0);

        return false;
    }

    auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();
    if (EffectDataTable == nullptr) {
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置效果表，请前往设置里配置")), 5.0);

        return false;
    }

    auto SkillGroupDataTable = SkillSetting->SkillGroupTable.LoadSynchronous();
    if (EffectDataTable == nullptr) {
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置技能模组表，请前往设置里配置")), 5.0);

        return false;
    }

    return true;
}

bool GameFrameworkEditorWidgetFactory_Skill::IsModelWindow() {
    return false;
}