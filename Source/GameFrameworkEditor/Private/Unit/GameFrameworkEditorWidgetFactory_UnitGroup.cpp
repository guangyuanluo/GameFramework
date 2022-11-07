#include "GameFrameworkEditorWidgetFactory_UnitGroup.h"
#include "SGameFrameworkWidget_UnitGroup.h"

#include "Modules/Unit/UnitSetting.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"

FText GameFrameworkEditorWidgetFactory_UnitGroup::GetDisplayName() {
	return FText::FromString(TEXT("单位组"));
}

FText GameFrameworkEditorWidgetFactory_UnitGroup::GetDisplayToolTip() {
	return FText::FromString(TEXT("编辑游戏中的单位组"));
}

FText GameFrameworkEditorWidgetFactory_UnitGroup::GetWindowName() {
	return FText::FromString(TEXT("单位组编辑器"));
}

TSharedRef<SWidget> GameFrameworkEditorWidgetFactory_UnitGroup::ConstructPage(TSharedPtr<FUICommandList> CommandList) {
	return SNew(SGameFrameworkWidget_UnitGroup, CommandList);
}

void GameFrameworkEditorWidgetFactory_UnitGroup::CheckEditorTableNoExistAndCreate() {

}

void GameFrameworkEditorWidgetFactory_UnitGroup::Export() {
	
}

bool GameFrameworkEditorWidgetFactory_UnitGroup::CheckOpenCondition() {
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置单位表，请前往设置里配置")), 5.0);

        return false;
    }
    auto UnitGroupDataTable = UnitSetting->UnitGroupTable.LoadSynchronous();
    if (UnitGroupDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置单位组表，请前往设置里配置")), 5.0);

        return false;
    }
    return true;
}