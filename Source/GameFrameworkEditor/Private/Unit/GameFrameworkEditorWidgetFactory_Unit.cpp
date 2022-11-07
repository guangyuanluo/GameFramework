#include "GameFrameworkEditorWidgetFactory_Unit.h"
#include "SGameFrameworkWidget_Unit.h"
#include "Modules/Unit/UnitSetting.h"
#include "Modules/Unit/UnitInfoConfigTableRow.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "AssetRegistryModule.h"
#include "UnrealEd/Public/ObjectTools.h"
#include "UnrealEd/Classes/Factories/DataAssetFactory.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"

FText GameFrameworkEditorWidgetFactory_Unit::GetDisplayName() {
	return FText::FromString(TEXT("单位"));
}

FText GameFrameworkEditorWidgetFactory_Unit::GetDisplayToolTip() {
	return FText::FromString(TEXT("编辑游戏中的单位信息"));
}

FText GameFrameworkEditorWidgetFactory_Unit::GetWindowName() {
	return FText::FromString(TEXT("单位编辑器"));
}

TSharedRef<SWidget> GameFrameworkEditorWidgetFactory_Unit::ConstructPage(TSharedPtr<FUICommandList> CommandList) {
	return SNew(SGameFrameworkWidget_Unit, CommandList);
}

void GameFrameworkEditorWidgetFactory_Unit::CheckEditorTableNoExistAndCreate() {

}

void GameFrameworkEditorWidgetFactory_Unit::Export() {
	
}

bool GameFrameworkEditorWidgetFactory_Unit::CheckOpenCondition() {
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置单位表，请前往设置里配置")), 5.0);

        return false;
    }
    return true;
}