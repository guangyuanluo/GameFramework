#include "GameFrameworkEditorWidgetFactory_Item.h"
#include "SGameFrameworkWidget_Item.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "Modules/Assets/BackpackSetting.h"
#include "Modules/Item/ItemSetting.h"
#include "GameFrameworkEditor.h"

FText GameFrameworkEditorWidgetFactory_Item::GetDisplayName() {
	return FText::FromString(TEXT("物品"));
}

FText GameFrameworkEditorWidgetFactory_Item::GetDisplayToolTip() {
	return FText::FromString(TEXT("编辑游戏中的物品相关信息"));
}

FText GameFrameworkEditorWidgetFactory_Item::GetWindowName() {
	return FText::FromString(TEXT("物品编辑器"));
}

TSharedRef<SWidget> GameFrameworkEditorWidgetFactory_Item::ConstructPage(TSharedPtr<FUICommandList> CommandList) {
	return SNew(SGameFrameworkWidget_Item, CommandList);
}

void GameFrameworkEditorWidgetFactory_Item::CheckEditorTableNoExistAndCreate() {
	
}

void GameFrameworkEditorWidgetFactory_Item::Export() {
}

bool GameFrameworkEditorWidgetFactory_Item::CheckOpenCondition() {
    const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
    auto BackpackTypeDataTable = BackpackSetting->BackpackTypeTable.LoadSynchronous();
    if (BackpackTypeDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置背包类型表，请前往设置里配置")), 5.0);

        return false;
    }

    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
    if (ItemTypeDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置物品类型表，请前往设置里配置")), 5.0);

        return false;
    }

    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    if (ItemDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置物品表，请前往设置里配置")), 5.0);

        return false;
    }

    return true;
}