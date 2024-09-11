// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkWidget_Unit.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SEditableLabel.h"
#include "Editor/DataTableEditor/Private/SRowEditor.h"
#include "SGameDataTableRowEditor.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "Modules/Unit/UnitInfoConfigTableRow.h"
#include "GameFrameworkEditorCommands.h"
#include "Modules/Exp/ExpSetting.h"
#include "Modules/Exp/ExpTypeConfigTableRow.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"

#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "Character/CoreCharacter.h"
#include "Modules/Unit/UnitSetting.h"
#include "SRowTableRefBox.h"
#include "Modules/Skill/SkillSetting.h"
#include "Modules/Skill/SkillGroupConfigTableRow.h"

const FString UnitDataDirectory = TEXT("/Data/Unit/");

namespace UnitInfoUI
{
	const FName UnitIdColumnName(TEXT("单位Id"));
	const FName UnitNameColumnName(TEXT("单位名字"));
    const FName UnitSkillGroupIdColumnName(TEXT("技能模组"));
};

class SUnitInfoRow : public SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>
{
public:
	SLATE_BEGIN_ARGS(SUnitInfoRow) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& Args, const TSharedRef<STableViewBase>& OwnerTableView, TSharedPtr<FConfigTableRowWrapper> InPtr, TSharedPtr<SGameDataTableRowEditor> InDataTableRowEditor)
	{
		UnitInfoPtr = InPtr;

		this->DataTableRowEditor = InDataTableRowEditor;

		SMultiColumnTableRow<TSharedPtr<FConfigTableRowWrapper>>::Construct(
			FSuperRowType::FArguments()
			.Padding(1.0f),
			OwnerTableView
		);
	}

	/** Overridden from SMultiColumnTableRow.  Generates a widget for this column of the list view. */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName) override
	{
        FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)(UnitInfoPtr->ConfigTableRow);
		if (ColumnName == UnitInfoUI::UnitIdColumnName)
		{
			return	SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(RowData->UnitId)));
		}
		else if (ColumnName == UnitInfoUI::UnitNameColumnName) {
			return	SNew(STextBlock)
				.Text(FText::FromString(RowData->UnitName));
		}
        else if (ColumnName == UnitInfoUI::UnitSkillGroupIdColumnName) {
            const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
            auto SkillGroupDataTable = SkillSetting->SkillGroupTable.LoadSynchronous();
            if (!SkillGroupDataTable) {
                return SNullWidget::NullWidget;
            }
            else {
                TSharedPtr<SRowTableRefBox> SkillGroupRefBox = SNew(SRowTableRefBox, SkillGroupDataTable, RowData->SkillGroupID);
                SkillGroupRefBox->RowSelectChanged.BindLambda([this](int ID) {
                    ((FUnitInfoConfigTableRow*)DataTableRowEditor->GetCurrentRow()->GetStructMemory())->SkillGroupID = ID;

                    DataTableRowEditor->MarkDatatableDirty();
                });
                return SkillGroupRefBox.ToSharedRef();
            }
        }

		return SNullWidget::NullWidget;
	}

private:
	TSharedPtr<FConfigTableRowWrapper> UnitInfoPtr;

	TArray<TSharedPtr<FConfigTableRowWrapper>> ExpTypeSource;
	TSharedPtr<FConfigTableRowWrapper> mSelectExpType;
	TSharedPtr<SGameDataTableRowEditor> DataTableRowEditor;
};

TArray<TSharedPtr<FConfigTableRowWrapper>> SGameFrameworkWidget_Unit::GetRowSource() {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    return GameFrameworkEditorModule.GetEditorWidgetTool()->GetUnitInfoSource();
}

class UDataTable* SGameFrameworkWidget_Unit::GetDataTable() {
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    return UnitSetting->UnitTable.LoadSynchronous();
}

FName SGameFrameworkWidget_Unit::NewRowInit(FName Name) {
    auto UnitInfoDataTable = GetDataTable();
    if (UnitInfoDataTable) {
        int32 NewId = ApplyUnitId();
        if (NewId != -1) {
            FName RowName = *FString::FromInt(NewId);
            if (FDataTableEditorUtils::RenameRow(UnitInfoDataTable, Name, RowName)) {
                FUnitInfoConfigTableRow* FindRow = UnitInfoDataTable->FindRow<FUnitInfoConfigTableRow>(RowName, "");
                FindRow->UnitId = NewId;
                FindRow->UnitName = TEXT("Unit_") + FString::FromInt(NewId);
                FindRow->Description = TEXT("这里填加描述");
                return RowName;
            }
        }
    }
    return FName();
}

int32 SGameFrameworkWidget_Unit::ApplyUnitId() {
	auto SeedTable = DataTableSeedConfigTableHelper::GetDataTable();
	auto UnitIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("UnitIdSeed", "");
	if (UnitIdSeed == nullptr) {
		FDataTableSeedConfigTableRow NewRow;
		NewRow.SeedKey = "UnitIdSeed";
		NewRow.SeedValue = 0;
		SeedTable->AddRow("UnitIdSeed", NewRow);
		UnitIdSeed = SeedTable->FindRow<FDataTableSeedConfigTableRow>("UnitIdSeed", "");
	}
	int NpcId = ++UnitIdSeed->SeedValue;
	SeedTable->MarkPackageDirty();
	return NpcId;
}

TSharedRef<ITableRow> SGameFrameworkWidget_Unit::ListViewOnGenerateRow(TSharedPtr<FConfigTableRowWrapper> Item, const TSharedRef<STableViewBase>& OwnerTable) {
    return SNew(SUnitInfoRow, OwnerTable, Item, EditorRow);
}

TSharedRef<class SHeaderRow> SGameFrameworkWidget_Unit::ConstructListViewHeadRow() {
    return SNew(SHeaderRow)
        + SHeaderRow::Column(UnitInfoUI::UnitIdColumnName)
        .DefaultLabel(FText::FromName(UnitInfoUI::UnitIdColumnName))
        .FillWidth(33.0f)
        + SHeaderRow::Column(UnitInfoUI::UnitNameColumnName)
        .DefaultLabel(FText::FromName(UnitInfoUI::UnitNameColumnName))
        .FillWidth(33.0f)
        +SHeaderRow::Column(UnitInfoUI::UnitSkillGroupIdColumnName)
        .DefaultLabel(FText::FromName(UnitInfoUI::UnitSkillGroupIdColumnName))
        .FillWidth(33.0f);
}

bool SGameFrameworkWidget_Unit::AllRowModified(FName Name, struct FConfigTableRowBase* ChangeRow, struct FConfigTableRowBase* OriginRow) {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    TArray<TSharedPtr<FConfigTableRowWrapper>> UnitInfoSource = SGameFrameworkWidget_Unit::GetRowSource();
    FUnitInfoConfigTableRow* FindRow = (FUnitInfoConfigTableRow*)ChangeRow;
    FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)OriginRow;
    //这里检查修改的是不是NPCName
    if (RowData->UnitName != FindRow->UnitName) {
        //如果改的是NPCName,进行非空检查和判重检查
        if (FindRow->UnitName.Len() == 0) {
            GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("UnitName不能为空，回滚操作")), 5.0f);
            return false;
        }
        for (auto Index2 = 0; Index2 < UnitInfoSource.Num(); ++Index2) {
            FUnitInfoConfigTableRow* RowData2 = (FUnitInfoConfigTableRow*)(UnitInfoSource[Index2]->ConfigTableRow);
            if (RowData2->UnitId != FindRow->UnitId
                && RowData2->UnitName == FindRow->UnitName) {
                GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("UnitName已经存在，回滚操作")), 5.0f);
                return false;
            }
        }
    }

    if (RowData->CharacterClass != FindRow->CharacterClass) {
        auto CharacterClass = FindRow->CharacterClass.LoadSynchronous();
        if (CharacterClass) {
            for (auto Index2 = 0; Index2 < UnitInfoSource.Num(); ++Index2) {
                FUnitInfoConfigTableRow* RowData2 = (FUnitInfoConfigTableRow*)(UnitInfoSource[Index2]->ConfigTableRow);
                if (RowData2->UnitId != FindRow->UnitId
                    && RowData2->CharacterClass == FindRow->CharacterClass) {
                    GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(FString::Format(TEXT("角色类已经被单位{0}占用，回滚操作"), { RowData2->UnitId })), 5.0f);
                    return false;
                }
            }
        }

        auto LoadClass = RowData->CharacterClass.LoadSynchronous();
        if (LoadClass) {
            auto OldCharacterAsset = Cast<ACoreCharacter>(LoadClass->GetDefaultObject());
            OldCharacterAsset->TemplateID = -1;
            OldCharacterAsset->MarkPackageDirty();
        }
    }
    if (FindRow->CharacterClass != NULL) {
        auto CharacterClass = FindRow->CharacterClass.LoadSynchronous();
        if (CharacterClass) {
            auto NewCharacterAsset = Cast<ACoreCharacter>(CharacterClass->GetDefaultObject());
            if (NewCharacterAsset->TemplateID != FindRow->UnitId) {
                NewCharacterAsset->TemplateID = FindRow->UnitId;
                NewCharacterAsset->MarkPackageDirty();
            }
        }
        else {
            GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("角色类加载失败")), 5.0f);
        }
    }
    return true;
}

bool SGameFrameworkWidget_Unit::AllowRowRemove(struct FConfigTableRowBase* RemoveRow) {
    FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
    FString UseInfo;
    FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)RemoveRow;
    if (GameFrameworkEditorModule.GetEditorWidgetTool()->IsUnitIdUse(RowData->UnitId, UseInfo)) {
        GameFrameworkEditorModule.GetEditorWidgetTool()->ShowNotification(FText::FromString(UseInfo), 5.0);
        return false;
    }
    return true;
}

void SGameFrameworkWidget_Unit::RowRemoved(struct FConfigTableRowBase* RemoveRow) {
    FUnitInfoConfigTableRow* RowData = (FUnitInfoConfigTableRow*)RemoveRow;
    TSoftClassPtr<ACoreCharacter> OrigineClass = RowData->CharacterClass;
    if (OrigineClass != NULL) {
        auto OldCharacterAsset = Cast<ACoreCharacter>(OrigineClass.Get()->GetDefaultObject());
        OldCharacterAsset->TemplateID = -1;
        OldCharacterAsset->MarkPackageDirty();
    }
}