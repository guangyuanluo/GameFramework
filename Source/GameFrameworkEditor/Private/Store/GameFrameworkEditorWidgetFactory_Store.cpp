#include "GameFrameworkEditorWidgetFactory_Store.h"
#include "SGameFrameworkWidget_StoreMain.h"
#include "Modules/Store/StoreSetting.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "AssetRegistryModule.h"
#include "UnrealEd/Public/ObjectTools.h"
#include "UnrealEd/Classes/Factories/DataAssetFactory.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"

FText GameFrameworkEditorWidgetFactory_Store::GetDisplayName() {
	return FText::FromString(TEXT("商店"));
}

FText GameFrameworkEditorWidgetFactory_Store::GetDisplayToolTip() {
	return FText::FromString(TEXT("编辑游戏中的商店和商品信息"));
}

FText GameFrameworkEditorWidgetFactory_Store::GetWindowName() {
	return FText::FromString(TEXT("商店商品编辑器"));
}

TSharedRef<SWidget> GameFrameworkEditorWidgetFactory_Store::ConstructPage(TSharedPtr<FUICommandList> CommandList) {
	return SNew(SGameFrameworkWidget_StoreMain, CommandList);
}

void GameFrameworkEditorWidgetFactory_Store::CheckEditorTableNoExistAndCreate() {

}

void GameFrameworkEditorWidgetFactory_Store::Export() {
	
}

bool GameFrameworkEditorWidgetFactory_Store::CheckOpenCondition() {
    const UStoreSetting* StoreSetting = GetDefault<UStoreSetting>();
    auto GoodsDataTable = StoreSetting->GoodsTable.LoadSynchronous();
    if (GoodsDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置商品表，请前往设置里配置")), 5.0);

        return false;
    }
    auto StoreDataTable = StoreSetting->StoreTable.LoadSynchronous();
    if (StoreDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置商店表，请前往设置里配置")), 5.0);

        return false;
    }
    return true;
}