#include "GameFrameworkEditorWidgetTool.h"

#include "Data/Reward/Reward/ExpReward.h"
#include "Data/Reward/Reward/MoneyReward.h"
#include "Data/Reward/Reward/ItemReward.h"
#include "Data/Reward/Reward/IntimacyReward.h"

#include "Global/Conditions/PlayerReachExpLevelCondition.h"
#include "Global/Conditions/UnitReachExpLevelCondition.h"
#include "Global/Conditions/PlayerCollectMoneyCondition.h"
#include "Global/Conditions/PlayerConsumeMoneyCondition.h"
#include "Global/Conditions/PlayerCollectItemCondition.h"
#include "Global/Conditions/PlayerConsumeItemCondition.h"

#include "Data/Reward/CoreReward.h"

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
#include "AssetRegistryModule.h"
#include "UnrealEd/Public/ObjectTools.h"
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
    auto SkilDataTable = SkillSetting->SkillTable.LoadSynchronous();
    if (SkilDataTable != nullptr) {
        auto TableUsingStruct = SkilDataTable->GetRowStruct();
        int32 StructureSize = TableUsingStruct->GetStructureSize();
        TArray<FConfigTableRowBase*> AllRowArr;
        SkilDataTable->GetAllRows("", AllRowArr);
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
	/*auto QuestMap = UQuestConfigTableHelper::GetAllQuestTemplates();
	for (auto QuestTemplatePair : QuestMap) {
		const auto& PreConditions = QuestTemplatePair.Value->GetPreConditions();
		for (auto PreCondition : PreConditions) {
            if (PreCondition->GetClass() == UPlayerReachExpLevelCondition::StaticClass()) {
                UPlayerReachExpLevelCondition* reachExpLevelCondition = (UPlayerReachExpLevelCondition*)PreCondition;
                if (reachExpLevelCondition->ExpType == ExpType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务前提条件中有使用该ExpType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
            else if (PreCondition->GetClass() == UUnitReachExpLevelCondition::StaticClass()) {
                UUnitReachExpLevelCondition* reachExpLevelCondition = (UUnitReachExpLevelCondition*)PreCondition;
                if (reachExpLevelCondition->ExpType == ExpType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务前提条件中有使用该ExpType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
			if (HaveFound) break;
		}
		if (HaveFound) break;

		const auto& CompleteConditions = QuestTemplatePair.Value->GetCompleteConditions();
		for (auto CompleteCondition : CompleteConditions) {
            if (CompleteCondition->GetClass() == UPlayerReachExpLevelCondition::StaticClass()) {
                UPlayerReachExpLevelCondition* reachExpLevelCondition = (UPlayerReachExpLevelCondition*)CompleteCondition;
                if (reachExpLevelCondition->ExpType == ExpType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务完成条件中有使用该ExpType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
            else if (CompleteCondition->GetClass() == UUnitReachExpLevelCondition::StaticClass()) {
                UUnitReachExpLevelCondition* reachExpLevelCondition = (UUnitReachExpLevelCondition*)CompleteCondition;
                if (reachExpLevelCondition->ExpType == ExpType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务完成条件中有使用该ExpType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
			if (HaveFound) break;
		}
		if (HaveFound) break;
		const auto& Rewards = QuestTemplatePair.Value->GetQuestRewards();
		for (auto Reward : Rewards) {
			switch (Reward->RewardType)
			{
			case RewardType::E_Exp: {
				UExpReward* ExpReward = (UExpReward*)Reward;
				if (ExpReward->ExpType == ExpType) {
					UseInfo = FString::Format(TEXT("questid为{0}的任务奖励中有使用该ExpType的"), { QuestTemplatePair.Key });
					HaveFound = true;
				}
			}
			default:
				break;
			}
			if (HaveFound) break;
		}
		if (HaveFound) break;
	}*/

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
	/*auto QuestMap = UQuestConfigTableHelper::GetAllQuestTemplates();
	for (auto QuestTemplatePair : QuestMap) {
		const auto& PreConditions = QuestTemplatePair.Value->GetPreConditions();
		for (auto PreCondition : PreConditions) {
            if (PreCondition->GetClass() == UPlayerCollectMoneyCondition::StaticClass()) {
                UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)PreCondition;
                if (CollectMoneyCondition->MoneyType == MoneyType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务前提条件中有使用该MoneyType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
            else if (PreCondition->GetClass() == UPlayerConsumeMoneyCondition::StaticClass()) {
                UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)PreCondition;
                if (ConsumeMoneyCondition->MoneyType == MoneyType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务前提条件中有使用该MoneyType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
			if (HaveFound) break;
		}
		if (HaveFound) break;

		const auto& CompleteConditions = QuestTemplatePair.Value->GetCompleteConditions();
		for (auto CompleteCondition : CompleteConditions) {
            if (CompleteCondition->GetClass() == UPlayerCollectMoneyCondition::StaticClass()) {
                UPlayerCollectMoneyCondition* CollectMoneyCondition = (UPlayerCollectMoneyCondition*)CompleteCondition;
                if (CollectMoneyCondition->MoneyType == MoneyType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务完成条件中有使用该MoneyType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
            else if (CompleteCondition->GetClass() == UPlayerConsumeMoneyCondition::StaticClass()) {
                UPlayerConsumeMoneyCondition* ConsumeMoneyCondition = (UPlayerConsumeMoneyCondition*)CompleteCondition;
                if (ConsumeMoneyCondition->MoneyType == MoneyType) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务完成条件中有使用该MoneyType的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
			if (HaveFound) break;
		}
		if (HaveFound) break;
		const auto& Rewards = QuestTemplatePair.Value->GetQuestRewards();
		for (auto Reward : Rewards) {
			switch (Reward->RewardType)
			{
			case RewardType::E_Money: {
				UMoneyReward* MoneyReward = (UMoneyReward*)Reward;
				if (MoneyReward->MoneyType == MoneyType) {
					UseInfo = FString::Format(TEXT("questid为{0}的任务奖励中有使用该MoneyType的"), { QuestTemplatePair.Key });
					HaveFound = true;
				}
			}
				break;
			default:
				break;
			}
			if (HaveFound) break;
		}
		if (HaveFound) break;
	}*/

	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsPackageTypeIdUse(int32 BackpackTypeId, FString& UseInfo) {
    bool HaveFound = false;
	/*auto QuestMap = UQuestConfigTableHelper::GetAllQuestTemplates();
	for (auto QuestTemplatePair : QuestMap) {
		const auto& Rewards = QuestTemplatePair.Value->GetQuestRewards();
		for (auto Reward : Rewards) {
			switch (Reward->RewardType)
			{
			case RewardType::E_Item: {
				UItemReward* ItemReward = (UItemReward*)Reward;
				if (ItemReward->BackpackType == BackpackTypeId) {
					UseInfo = FString::Format(TEXT("questid为{0}的任务奖励中有使用该背包类型的"), { QuestTemplatePair.Key });
					HaveFound = true;
				}
			}
				break;
			default:
				break;
			}
			if (HaveFound) break;
		}
		if (HaveFound) break;
	}*/

	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsItemIdUse(int32 ItemId, FString& UseInfo) {
    bool HaveFound = false;
	/*auto QuestMap = UQuestConfigTableHelper::GetAllQuestTemplates();
	for (auto QuestTemplatePair : QuestMap) {
		const auto& PreConditions = QuestTemplatePair.Value->GetPreConditions();
		for (auto PreCondition : PreConditions) {
            if (PreCondition->GetClass() == UPlayerCollectItemCondition::StaticClass()) {
                UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)PreCondition;
                if (CollectItemCondition->ItemId == ItemId) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务前提条件中有使用该ItemId的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
            else if (PreCondition->GetClass() == UPlayerConsumeItemCondition::StaticClass()) {
                UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)PreCondition;
                if (ConsumeItemCondition->ItemId == ItemId) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务前提条件中有使用该ItemId的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
			if (HaveFound) break;
		}
		if (HaveFound) break;

		const auto& CompleteConditions = QuestTemplatePair.Value->GetCompleteConditions();
		for (auto CompleteCondition : CompleteConditions) {
            if (CompleteCondition->GetClass() == UPlayerCollectItemCondition::StaticClass()) {
                UPlayerCollectItemCondition* CollectItemCondition = (UPlayerCollectItemCondition*)CompleteCondition;
                if (CollectItemCondition->ItemId == ItemId) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务完成条件中有使用该ItemId的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
            else if (CompleteCondition->GetClass() == UPlayerConsumeItemCondition::StaticClass()) {
                UPlayerConsumeItemCondition* ConsumeItemCondition = (UPlayerConsumeItemCondition*)CompleteCondition;
                if (ConsumeItemCondition->ItemId == ItemId) {
                    UseInfo = FString::Format(TEXT("questid为{0}的任务完成条件中有使用该ItemId的"), { QuestTemplatePair.Key });
                    HaveFound = true;
                }
            }
			if (HaveFound) break;
		}
		if (HaveFound) break;
		const auto& Rewards = QuestTemplatePair.Value->GetQuestRewards();
		for (auto Reward : Rewards) {
			switch (Reward->RewardType)
			{
			case RewardType::E_Item: {
				UItemReward* ItemReward = (UItemReward*)Reward;
				if (ItemReward->ItemId == ItemId) {
					UseInfo = FString::Format(TEXT("questid为{0}的任务奖励中有使用该ItemId的"), { QuestTemplatePair.Key });
					HaveFound = true;
				}
			}
			default:
				break;
			}
			if (HaveFound) break;
		}
		if (HaveFound) break;
	}*/

	return HaveFound;
}

bool GameFrameworkEditorWidgetTool::IsUnitIdUse(int32 UnitId, FString& UseInfo) {
    bool HaveFound = false;
	/*auto QuestMap = UQuestConfigTableHelper::GetAllQuestTemplates();
	for (auto QuestTemplatePair : QuestMap) {
		if (QuestTemplatePair.Value->GetUnitConsigner() == UnitId) {
			UseInfo = FString::Format(TEXT("questid为{0}的任务委托的NPC中有使用该单位的"), { QuestTemplatePair.Key });
			HaveFound = true;
			break;
		}
		if (QuestTemplatePair.Value->GetUnitSubmit() == UnitId) {
			UseInfo = FString::Format(TEXT("questid为{0}的任务提交的NPC中有使用该单位的"), { QuestTemplatePair.Key });
			HaveFound = true;
			break;
		}
		const auto& Rewards = QuestTemplatePair.Value->GetQuestRewards();
		for (auto Reward : Rewards) {
			switch (Reward->RewardType)
			{
			case RewardType::E_Intimacy: {
				UIntimacyReward* IntimacyReward = (UIntimacyReward*)Reward;
				if (IntimacyReward->UnitId == UnitId) {
					UseInfo = FString::Format(TEXT("questid为{0}的任务奖励中有使用该单位的"), { QuestTemplatePair.Key });
					HaveFound = true;
				}
			}
			default:
				break;
			}
			if (HaveFound) break;
		}
		if (HaveFound) break;
	}*/
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

bool GameFrameworkEditorWidgetTool::IsSkillIdUse(int32 SkillId, FString& UseInfo) {
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

	FString AssetPath = FString::Format(TEXT("/Game/Data/Enum/{0}.{1}"), { EnumClassName, EnumClassName });
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	auto FindAsset = AssetRegistryModule.Get().GetAssetByObjectPath(*AssetPath);
	if (!FindAsset.IsValid()) {
		UGenerateEnumFactory* Factory = NewObject<UGenerateEnumFactory>();
		AssetToolsModule.Get().CreateAsset(EnumClassName, Directory, UGenerateEnum::StaticClass(), Factory);
		FindAsset = AssetRegistryModule.Get().GetAssetByObjectPath(*AssetPath);
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