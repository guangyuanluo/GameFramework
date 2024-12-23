#include "GameFrameworkEditorWidgetFactory_Exp.h"
#include "Modules/Exp/ExpSetting.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"

FText GameFrameworkEditorWidgetFactory_Exp::GetDisplayName() {
	return FText::FromString(TEXT("经验"));
}

FText GameFrameworkEditorWidgetFactory_Exp::GetDisplayToolTip() {
	return FText::FromString(TEXT("编辑游戏中的经验类型"));
}

FText GameFrameworkEditorWidgetFactory_Exp::GetWindowName() {
	return FText::FromString(TEXT("经验编辑器"));
}

TSharedRef<SWidget> GameFrameworkEditorWidgetFactory_Exp::ConstructPage(TSharedPtr<FUICommandList> CommandList) {
	return SNullWidget::NullWidget;
}

void GameFrameworkEditorWidgetFactory_Exp::CheckEditorTableNoExistAndCreate() {

}

void GameFrameworkEditorWidgetFactory_Exp::Export() {

}

bool GameFrameworkEditorWidgetFactory_Exp::CheckOpenCondition() {
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();

    if (ExpTypeDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置经验类型表，请前往设置里配置")), 5.0);

        return false;
    }
    return true;
}

void GameFrameworkEditorWidgetFactory_Exp::Open(TSharedPtr<FUICommandList> InCommandList) {
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();
    GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(ExpTypeDataTable);
}