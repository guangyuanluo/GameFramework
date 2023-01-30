#include "GameFrameworkEditorWidgetTool.h"

#include "Global/Conditions/PlayerReachExpLevelCondition.h"
#include "Global/Conditions/UnitReachExpLevelCondition.h"
#include "Global/Conditions/PlayerCollectMoneyCondition.h"
#include "Global/Conditions/PlayerConsumeMoneyCondition.h"
#include "Global/Conditions/PlayerCollectItemCondition.h"
#include "Global/Conditions/PlayerConsumeItemCondition.h"

#include "Runtime/Slate/Public/Framework/Notifications/NotificationManager.h"
#include "Runtime/Slate/Public/Widgets/Notifications/SNotificationList.h"

#include "Modules/Money/MoneySetting.h"
#include "Modules/Exp/ExpSetting.h"
#include "Modules/Assets/BackpackSetting.h"
#include "Modules/Item/ItemSetting.h"
#include "Modules/Unit/UnitSetting.h"
#include "Modules/Skill/SkillSetting.h"
#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "Modules/Unit/UnitInfoConfigTableRow.h"
#include "Modules/Unit/UnitGroupConfigTableRow.h"
#include "Modules/Item/CoreItem.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Kismet2/EnumEditorUtils.h"
#if ENGINE_MAJOR_VERSION > 4
#include "AssetRegistry/AssetRegistryModule.h"
#else
#include "AssetRegistryModule.h"
#endif
#include "ObjectTools.h"
#include "Editor/Enum/GenerateEnum.h"
#include "Enum/GenerateEnumFactory.h"

void GameFrameworkEditorWidgetTool::ShowNotification(const FText& TextToDisplay, float TimeToDisplay) {
	FNotificationInfo Info(TextToDisplay);
	Info.ExpireDuration = TimeToDisplay;

	FSlateNotificationManager::Get().AddNotification(Info);
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetUnitInfoSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable != nullptr) {
        auto TableUsingStruct = UnitDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FUnitInfoConfigTableRow*> UnitInfoArr;
        UnitDataTable->GetAllRows("", UnitInfoArr);
        for (auto Index = 0; Index < UnitInfoArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, UnitInfoArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetUnitGroupSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitGroupDataTable = UnitSetting->UnitGroupTable.LoadSynchronous();
    if (UnitGroupDataTable != nullptr) {
        auto TableUsingStruct = UnitGroupDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FConfigTableRowBase*> UnitGroupArr;
        UnitGroupDataTable->GetAllRows("", UnitGroupArr);
        for (auto Index = 0; Index < UnitGroupArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, UnitGroupArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetMoneyTypeSource() {
	TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UMoneySetting* MoneySetting = GetDefault<UMoneySetting>();
    auto MoneyTypeDataTable = MoneySetting->MoneyTypeTable.LoadSynchronous();
    if (MoneyTypeDataTable != nullptr) {
        auto TableUsingStruct = MoneyTypeDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
		TArray<FMoneyTypeConfigTableRow*> MoneyTypeArr;
        MoneyTypeDataTable->GetAllRows("", MoneyTypeArr);
		for (auto Index = 0; Index < MoneyTypeArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, MoneyTypeArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
		}
	}

	return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetExpTypeSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();
    if (ExpTypeDataTable != nullptr) {
        auto TableUsingStruct = ExpTypeDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FExpTypeConfigTableRow*> ExpTypeArr;
        ExpTypeDataTable->GetAllRows("", ExpTypeArr);
        for (auto Index = 0; Index < ExpTypeArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, ExpTypeArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetBackpackTypeSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
    auto BackpackTypeDataTable = BackpackSetting->BackpackTypeTable.LoadSynchronous();
    if (BackpackTypeDataTable != nullptr) {
        auto TableUsingStruct = BackpackTypeDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FBackpackTypeConfigTableRow*> BackpackTypeArr;
        BackpackTypeDataTable->GetAllRows("", BackpackTypeArr);
        for (auto Index = 0; Index < BackpackTypeArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, BackpackTypeArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetItemInfoSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    if (ItemDataTable != nullptr) {
        auto TableUsingStruct = ItemDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FItemConfigTableRow*> ItemArr;
        ItemDataTable->GetAllRows("", ItemArr);
        for (auto Index = 0; Index < ItemArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, ItemArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetItemTypeSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
    if (ItemTypeDataTable != nullptr) {
        auto TableUsingStruct = ItemTypeDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FItemTypeConfigTableRow*> ItemTypeArr;
        ItemTypeDataTable->GetAllRows("", ItemTypeArr);
        for (auto Index = 0; Index < ItemTypeArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, ItemTypeArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetSkillSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    if (SkillDataTable != nullptr) {
        auto TableUsingStruct = SkillDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FConfigTableRowBase*> AllRowArr;
        SkillDataTable->GetAllRows("", AllRowArr);
        for (auto Index = 0; Index < AllRowArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, AllRowArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetEffectSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();
    if (EffectDataTable != nullptr) {
        auto TableUsingStruct = EffectDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FConfigTableRowBase*> AllRowArr;
        EffectDataTable->GetAllRows("", AllRowArr);
        for (auto Index = 0; Index < AllRowArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, AllRowArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

TArray<TSharedPtr<FConfigTableRowWrapper>> GameFrameworkEditorWidgetTool::GetSkillGroupSource() {
    TArray<TSharedPtr<FConfigTableRowWrapper>> Result;
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillGroupDataTable = SkillSetting->SkillGroupTable.LoadSynchronous();
    if (SkillGroupDataTable != nullptr) {
        auto TableUsingStruct = SkillGroupDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FConfigTableRowBase*> AllRowArr;
        SkillGroupDataTable->GetAllRows("", AllRowArr);
        for (auto Index = 0; Index < AllRowArr.Num(); ++Index) {
            FConfigTableRowWrapper* NewWrapper = new FConfigTableRowWrapper();
            uint8* NewRawRowData = (uint8*)FMemory::Malloc(StructureSize);

            TableUsingStruct->InitializeStruct(NewRawRowData);
            TableUsingStruct->CopyScriptStruct(NewRawRowData, AllRowArr[Index]);

            NewWrapper->RowStruct = TableUsingStruct;
            NewWrapper->ConfigTableRow = NewRawRowData;

            Result.Add(TSharedPtr<FConfigTableRowWrapper>(NewWrapper));
        }
    }

    return Result;
}

bool GameFrameworkEditorWidgetTool::IsExpTypeUse(int32 ExpType, FString& UseInfo) {
	if (ExpType == FExpTypeConfigTableRow::RoleExpType) {
		UseInfo = TEXT("默认经验类型不能删除");
		return true;
	}

    bool HaveFound = false;

    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable) {
        UnitDataTable->ForeachRow<FUnitInfoConfigTableRow>("", [this, &ExpType, &HaveFound, &UseInfo](const FName& key, const FUnitInfoConfigTableRow& Value) -> void {
            if (HaveFound) return;
            if (Value.GrowExpTypeId == ExpType) {
                HaveFound = true;
                UseInfo = FString::Format(TEXT("单位表中有使用这个经验类型的，单位Id:{0}"), { Value.UnitId });
            }
        });
    }

	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsMoneyTypeUse(int32 MoneyType, FString& UseInfo) {
    bool HaveFound = false;

	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsPackageTypeIdUse(int32 BackpackTypeId, FString& UseInfo) {
    bool HaveFound = false;

	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsItemIdUse(int32 ItemId, FString& UseInfo) {
    bool HaveFound = false;
	
	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsUnitIdUse(int32 UnitId, FString& UseInfo) {
    bool HaveFound = false;
	if (!HaveFound)
	{
        const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
        auto UnitGroupDataTable = UnitSetting->UnitGroupTable.LoadSynchronous();
		if (UnitGroupDataTable) {
			TArray<FUnitGroupConfigTableRow*> UnitGroupArr;
			UnitGroupDataTable->GetAllRows("", UnitGroupArr);
			for (int Index = 0; Index < UnitGroupArr.Num(); ++Index)
			{
				if (UnitGroupArr[Index]->Units.Contains(UnitId))
				{
					UseInfo = FString::Format(TEXT("单位组{0}中有使用该单位的"), { UnitGroupArr[Index]->GroupId });
					HaveFound = true;
					break;
				}
			}
		}
	}

	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsSkillGroupIdUse(int32 SkillId, FString& UseInfo) {
    return false;
}

bool GameFrameworkEditorWidgetTool::IsItemIdExist(int32 ItemId) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    if (ItemDataTable) {
        TArray<FItemConfigTableRow*> ItemArr;
        ItemDataTable->GetAllRows("", ItemArr);
        for (auto Index = 0; Index < ItemArr.Num(); ++Index) {
            if (ItemArr[Index]->ItemId == ItemId) {
                return true;
            }
        }
    }
    return false;
}

bool GameFrameworkEditorWidgetTool::IsUnitIdExist(int32 UnitId) {
    bool HaveFound = false;

    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable) {
        TArray<FUnitInfoConfigTableRow*> UnitArr;
        UnitDataTable->GetAllRows("", UnitArr);
        for (auto Index = 0; Index < UnitArr.Num(); ++Index) {
            if (UnitArr[Index]->UnitId == UnitId) {
                HaveFound = true;
                break;
            }
        }
    }
	return HaveFound;
}

void GameFrameworkEditorWidgetTool::ExportEnumBP(FString EnumClassName, FText EnumDisplayName, TMap<int64, TPair<FString, FText>> EnumSource) {
	const FString Directory = TEXT("/Game/Data/Enum");
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Directory)) {
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*Directory);
	}

#if ENGINE_MAJOR_VERSION > 4
    FSoftObjectPath AssetPath = FString::Format(TEXT("/Game/Data/Enum/{0}.{1}"), { EnumClassName, EnumClassName });
#else
    FName AssetPath = *FString::Format(TEXT("/Game/Data/Enum/{0}.{1}"), { EnumClassName, EnumClassName });
#endif
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	auto FindAsset = AssetRegistryModule.Get().GetAssetByObjectPath(AssetPath);
	if (!FindAsset.IsValid()) {
		UGenerateEnumFactory* Factory = NewObject<UGenerateEnumFactory>();
		AssetToolsModule.Get().CreateAsset(EnumClassName, Directory, UGenerateEnum::StaticClass(), Factory);
		FindAsset = AssetRegistryModule.Get().GetAssetByObjectPath(AssetPath);
	}
	UGenerateEnum* Enum = Cast<UGenerateEnum>(FindAsset.GetAsset());
	TMap<int64, FText> OldEnumValueMap;
	for (int Index = 0; Index < Enum->NumEnums() - 1; ++Index) {
		auto EnumValue = Enum->GetValueByIndex(Index);

		OldEnumValueMap.Add(EnumValue, Enum->GetDisplayNameTextByIndex(Index));
	}

	TArray<TPair<FName, int64>> EnumNameArr;
	TMap<int32, FText> EnumDisplayMap;
	int MaxEnumId = 0;
	for (auto Iter = EnumSource.CreateConstIterator(); Iter; ++Iter) {
		EnumNameArr.Add(TPair<FName, int64>(*Enum->GenerateFullEnumName(*Iter->Value.Key), Iter->Key));
		EnumDisplayMap.Add(Iter->Key, Iter->Value.Value);
		if (MaxEnumId < Iter->Key) {
			MaxEnumId = Iter->Key;
		}
	}

	if (EnumDisplayName.ToString() == Enum->EnumDescription.ToString() && OldEnumValueMap.Num() == EnumDisplayMap.Num()) {
		bool IsSame = true;
		for (auto Iter = OldEnumValueMap.CreateConstIterator(); Iter; ++Iter) {
			if (!EnumDisplayMap.Contains(Iter->Key)) {
				IsSame = false;
				break;
			}
			if (EnumDisplayMap[Iter->Key].ToString() != Iter->Value.ToString()) {
				IsSame = false;
				break;
			}
		}
		if (IsSame) {
			return;
		}
	}
	Enum->EnumDescription = EnumDisplayName;

	Enum->UniqueNameIndex = MaxEnumId;
	Enum->SetEnumsEditor(EnumNameArr, UEnum::ECppForm::Namespaced);
	Enum->DisplayNameMap.Empty();
	//这个地方用API，触发其他蓝图刷新
	const int32 EnumeratorsToCopy = Enum->NumEnums() - 1;
	for (int32 Index = 0; Index < EnumeratorsToCopy; Index++)
	{
		auto EnumValue = Enum->GetValueByIndex(Index);
		auto displayName = EnumDisplayMap[EnumValue];
		FEnumEditorUtils::SetEnumeratorDisplayName(Enum, Index, displayName);
	}

	FEnumEditorUtils::EnsureAllDisplayNamesExist(Enum);
	Enum->MarkPackageDirty();
}