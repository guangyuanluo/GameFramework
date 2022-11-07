#include "GameFrameworkEditorWidgetFactory_Money.h"
#include "SGameFrameworkWidget_Money.h"
#include "Modules/Money/MoneySetting.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"


FText GameFrameworkEditorWidgetFactory_Money::GetDisplayName() {
	return FText::FromString(TEXT("货币"));
}

FText GameFrameworkEditorWidgetFactory_Money::GetDisplayToolTip() {
	return FText::FromString(TEXT("编辑游戏中的货币类型"));
}

FText GameFrameworkEditorWidgetFactory_Money::GetWindowName() {
	return FText::FromString(TEXT("货币编辑器"));
}

TSharedRef<SWidget> GameFrameworkEditorWidgetFactory_Money::ConstructPage(TSharedPtr<FUICommandList> CommandList) {
	return SNew(SGameFrameworkWidget_Money, CommandList);
}

void GameFrameworkEditorWidgetFactory_Money::CheckEditorTableNoExistAndCreate() {
    
}

void GameFrameworkEditorWidgetFactory_Money::Export() {
	TMap<int64, TPair<FString, FText>> EnumSource;

    const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
    auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();

    if (MoneyTypeDataTable) {
        TArray<FMoneyTypeConfigTableRow*> AllRows;
        MoneyTypeDataTable->GetAllRows("", AllRows);

        for (int Index = 0; Index < AllRows.Num(); ++Index) {
            auto moneyTypeName = FString::Format(TEXT("MoneyType_{0}"), { AllRows[Index]->MoneyTypeId });
            auto moneyTypeDescription = AllRows[Index]->MoneyTypeDescription;
            EnumSource.Add(AllRows[Index]->MoneyTypeId, TPair<FString, FText>(moneyTypeName, FText::FromString(moneyTypeDescription)));
        }

        GameFrameworkEditorWidgetTool::ExportEnumBP(TEXT("MoneyTypeEnum"), FText::FromString(TEXT("货币类型")), EnumSource);
    }
}

bool GameFrameworkEditorWidgetFactory_Money::CheckOpenCondition() {
    const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
    auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();

    if (MoneyTypeDataTable == nullptr) {
        FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("没配置货币类型表，请前往设置里配置")), 5.0);

        return false;
    }
    return true;
}