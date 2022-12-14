// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetSystem.h"
#include "Engine/World.h"
#include "CoreGameInstance.h"
#include "EventSystem.h"
#include "ChangeItemEvent.h"
#include "ConsumeItemEvent.h"
#include "ChangeMoneyEvent.h"
#include "ConsumeMoneyEvent.h"
#include "UE4LogImpl.h"
#include "CoreItem.h"
#include "CoreSceneItem.h"
#include "WalletComponent.h"
#include "ItemComponent.h"
#include "AssetBackpack.h"
#include "Kismet/GameplayStatics.h"
#include "CoreCharacter.h"
#include "CorePlayerController.h"
#include "AbilitySystemComponent.h"
#include "GameEntityManager.h"
#include "GameSystemManager.h"
#include "GameFrameworkUtils.h"
#include "ItemSortPredicate.h"
#include "ItemSetting.h"
#include "BackpackSetting.h"
#include "BackpackExtendHandler.h"
#include "CoreCharacterStateBase.h"
#include "ConfigTableCache.h"
#include "ItemConfigTableRow.h"
#include "BackpackTypeConfigTableRow.h"
#include "ItemTypeConfigTableRow.h"
#include "ItemIDNumPair.h"

void UAssetSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);

	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->RegistEventHandler(this);
}

void UAssetSystem::Uninitialize() {
	Super::Uninitialize();
}

int32 UAssetSystem::AddItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot, bool Force, const FString& Reason, FString& Error) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
    if (ItemDataTable && ItemTypeDataTable) {
        auto AddItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
        if (AddItemInfo) {
            if (BackpackType == FBackpackTypeConfigTableRow::BackpackTypeMax) {
                auto AddItemTypeInfo = (FItemTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemTypeDataTable, AddItemInfo->ItemType);
                if (AddItemTypeInfo) {
                    BackpackType = AddItemTypeInfo->DefaultBackpackType;
                }
            }

            auto Result = SimulateAddItem(BackpackComponent, BackpackType, ItemId, Count, SpecialSlot, Force, Reason, Error);
            if (Error.IsEmpty()) {
                auto& Package = MakePackageExist(BackpackComponent, BackpackType);
                auto& Backpack = Package.ItemList;

                auto ItemClass = UCoreItem::StaticClass();
                if (AddItemInfo->ItemClass) {
                    ItemClass = AddItemInfo->ItemClass;
                }

                int AddCount = 0;
                for (auto Iter = Result.CreateConstIterator(); Iter; ++Iter) {
                    if (Backpack[Iter->Key] == nullptr) {
                        auto AddItem = GenerateNewItem(BackpackComponent, ItemId, ItemClass);
                        Backpack[Iter->Key] = AddItem;

                        OnItemEnterPackage(BackpackComponent, AddItem, BackpackType, Iter->Key);

                        AddCount += Iter->Value;
                    }
                    else {
                        AddCount += Iter->Value - Backpack[Iter->Key]->ItemNum;
                    }
                    Backpack[Iter->Key]->ItemNum = Iter->Value;
                }
                SendChangeItemEvent(BackpackComponent, ItemId, AddCount, Reason);
                if (Force) {
                    //todo, ?????????????????????????????????????????????????????????????????????
                    if (AddCount != Count) {
                        AddCount = Count;
                    }
                }
                BackpackComponent->NotifyBackpackChanged();

                UE_LOG(GameCore, Log, TEXT("?????????????????????Itemid[%d],Num[%d]"), ItemId, Count);

                return AddCount;
            }
        }
        else {
            UE_LOG(GameCore, Warning, TEXT("??????????????????????????????????????????id"));
        }
    }
    else {
        UE_LOG(GameCore, Warning, TEXT("??????????????????????????????????????????"));
    }
    UE_LOG(GameCore, Log, TEXT("?????????????????????Itemid[%d],Num[%d]"), ItemId, Count);
	return 0;
}

bool UAssetSystem::CanAddItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot) {
    FString Error;
    auto Result = SimulateAddItem(BackpackComponent, BackpackType, ItemId, Count, SpecialSlot, false, "", Error);
    return Error.IsEmpty();
}

bool UAssetSystem::AddItems(UBackpackComponent* BackpackComponent, const TArray<FAddItemInfo>& AddItems, bool Force, const FString& Reason, FString& Error) {
    auto Result = SimulateAddItems(BackpackComponent, AddItems, Force, Reason, Error);
    if (Error.IsEmpty()) {
        const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
        auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
        if (ItemDataTable) {
            for (auto PackageIter = Result.CreateConstIterator(); PackageIter; ++PackageIter) {
                auto BackpackType = PackageIter->Key;
                auto& Package = MakePackageExist(BackpackComponent, BackpackType);
                auto& Backpack = Package.ItemList;

                for (auto ItemIter = PackageIter->Value.CreateConstIterator(); ItemIter; ++ItemIter) {
                    auto ItemId = ItemIter->Key;
                    auto AddItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
                    auto ItemClass = UCoreItem::StaticClass();
                    if (AddItemInfo->ItemClass) {
                        ItemClass = AddItemInfo->ItemClass;
                    }
                    int AddCount = 0;
                    for (auto IndexCountIter = ItemIter->Value.CreateConstIterator(); IndexCountIter; ++IndexCountIter) {
                        if (Backpack[IndexCountIter->Key] == nullptr) {
                            auto AddItem = GenerateNewItem(BackpackComponent, ItemId, ItemClass);
                            Backpack[IndexCountIter->Key] = AddItem;

                            OnItemEnterPackage(BackpackComponent, AddItem, BackpackType, IndexCountIter->Key);

                            AddCount += IndexCountIter->Value;
                        }
                        else {
                            AddCount += IndexCountIter->Value - Backpack[IndexCountIter->Key]->ItemNum;
                        }
                        Backpack[IndexCountIter->Key]->ItemNum = IndexCountIter->Value;
                    }
                    SendChangeItemEvent(BackpackComponent, ItemId, AddCount, Reason);
                    if (Force) {
                        //todo, ?????????????????????????????????????????????????????????????????????

                    }
                }
            }
            BackpackComponent->NotifyBackpackChanged();

            return true;
        }
        else {
            UE_LOG(GameCore, Warning, TEXT("??????????????????????????????????????????"));
        }
    }

    return false;
}

bool UAssetSystem::CanAddItems(UBackpackComponent* BackpackComponent, const TArray<FAddItemInfo>& AddItems) {
    FString Error;
    auto Result = SimulateAddItems(BackpackComponent, AddItems, false, "", Error);
    return Error.IsEmpty();
}

bool UAssetSystem::UseItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason) {
	bool Result = UseItemPrivate(BackpackComponent, BackpackType, SlotIndex, Count, Reason);
	if (Result) {
		BackpackComponent->NotifyBackpackChanged();
	}
	return Result;
}

UCoreItem* UAssetSystem::AbandonItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason, FString& Error) {
	auto Result = AbandonItemPrivate(BackpackComponent, BackpackType, SlotIndex, Count, Reason, Error);

	if (Error.IsEmpty()) {
		BackpackComponent->NotifyBackpackChanged();
	}

	return Result;
}

bool UAssetSystem::DeductItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int Count, int32 SpecialSlot, const FString& Reason, FString& Error) {
	TMap<int, TMap<int, int>> Result = SimulateDeductItem(BackpackComponent, BackpackType, ItemId, Count, SpecialSlot, Reason, Error);

	if (Error.IsEmpty()) {
        for (auto PackageIter = Result.CreateConstIterator(); PackageIter; ++PackageIter) {
            auto& Package = BackpackComponent->GetBackpack(PackageIter->Key);
            auto& Backpack = Package.ItemList;

            for (auto ItemIter = PackageIter->Value.CreateConstIterator(); ItemIter; ++ItemIter) {
                if (Backpack[ItemIter->Key]) {
                    Backpack[ItemIter->Key]->ItemNum -= ItemIter->Value;
                    if (Backpack[ItemIter->Key]->ItemNum <= 0) {
                        OnItemLeavePackage(BackpackComponent, Backpack[ItemIter->Key], PackageIter->Key, ItemIter->Key);
                        Backpack[ItemIter->Key] = nullptr;
                    }
                    SendChangeItemEvent(BackpackComponent, ItemId, -Count, Reason);
                }
            }
        }

		BackpackComponent->NotifyBackpackChanged();

        UE_LOG(GameCore, Log, TEXT("?????????????????????itemid[%d],num[%d]"), ItemId, Count);

        return true;
	}

    UE_LOG(GameCore, Log, TEXT("?????????????????????itemid[%d],num[%d]"), ItemId, Count);
	return false;
}

bool UAssetSystem::CanDeductItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int Count, int32 SpecialSlot) {
    FString Error;
    TMap<int, TMap<int, int>> Result = SimulateDeductItem(BackpackComponent, BackpackType, ItemId, Count, SpecialSlot, "", Error);
    return Error.IsEmpty();
}

bool UAssetSystem::DeductItems(UBackpackComponent* BackpackComponent, const TArray<FItemIDNumPair>& DeductItems, const FString& Reason, FString& Error) {
    auto Result = SimulateDeductItems(BackpackComponent, DeductItems, Reason, Error);
    if (Error.IsEmpty()) {
        for (auto PackageIter = Result.CreateConstIterator(); PackageIter; ++PackageIter) {
            auto BackpackType = PackageIter->Key;
            auto& Package = MakePackageExist(BackpackComponent, BackpackType);
            auto& Backpack = Package.ItemList;

            for (auto ItemIter = PackageIter->Value.CreateConstIterator(); ItemIter; ++ItemIter) {
                auto ItemId = ItemIter->Key;
                for (auto IndexCountIter = ItemIter->Value.CreateConstIterator(); IndexCountIter; ++IndexCountIter) {
                    Backpack[IndexCountIter->Key]->ItemNum -= IndexCountIter->Value;
                    if (Backpack[IndexCountIter->Key]->ItemNum == 0) {
                        OnItemLeavePackage(BackpackComponent, Backpack[IndexCountIter->Key], BackpackType, IndexCountIter->Key);
                        Backpack[IndexCountIter->Key] = nullptr;
                    }
                    SendChangeItemEvent(BackpackComponent, ItemId, -IndexCountIter->Value, Reason);
                }
            }
        }
        BackpackComponent->NotifyBackpackChanged();

        return true;
    }

    return false;
}

bool UAssetSystem::CanDeductItems(UBackpackComponent* BackpackComponent, const TArray<FItemIDNumPair>& DeductItems) {
    FString Error;
    auto Result = SimulateDeductItems(BackpackComponent, DeductItems, "", Error);
    return Error.IsEmpty();
}

bool UAssetSystem::MoveItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int32 NewPackageType, int NewSlotIndex, FString& Error) {
	bool Result = MoveItemPrivate(BackpackComponent, BackpackType, SlotIndex, NewPackageType, NewSlotIndex, Error);

	if (Error.IsEmpty()) {
		BackpackComponent->NotifyBackpackChanged();
	}

	return Result;
}

int32 UAssetSystem::SplitItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, FString& Error) {
	auto Result = SplitItemPrivate(BackpackComponent, BackpackType, SlotIndex, Count, Error);

	if (Error.IsEmpty()) {
		BackpackComponent->NotifyBackpackChanged();
	}

	return Result;
}

void UAssetSystem::SortBackpack(UBackpackComponent* BackpackComponent, uint8 BackpackType) {
    auto& assetBackpack = BackpackComponent->GetBackpack(BackpackType);
    if (UAssetBackpackBlueprintLibrary::IsValid(assetBackpack)) {
        const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
        const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
        auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();

        TSubclassOf<UItemSortPredicate> ItemSortPredicateClass = UItemSortPredicate::StaticClass(); 
        FString ItemSortPredicateClassPath = BackpackSetting->ItemSortPredicateClass.ToString();
        if (!ItemSortPredicateClassPath.IsEmpty()) {
            TSubclassOf<UItemSortPredicate> LoadClass = StaticLoadClass(UItemSortPredicate::StaticClass(), NULL, *ItemSortPredicateClassPath);
            if (LoadClass) {
                ItemSortPredicateClass = LoadClass;
            }
        }
        auto ItemSortPredicate = Cast<UItemSortPredicate>(ItemSortPredicateClass->GetDefaultObject());
        TArray<TLinkedList<UCoreItem*>*> AllNodes;
        TLinkedList<UCoreItem*>* Head = nullptr;
        auto& Backpack = assetBackpack.ItemList;
        //???????????????,??????????????????????????????????????????????????????id?????????????????????????????????
        for (int Index = 0; Index < Backpack.Num(); ++Index) {
            if (Backpack[Index]) {
                if (AllNodes.Num() == 0) {
                    TLinkedList<UCoreItem*>* First = new TLinkedList<UCoreItem*>(Backpack[Index]);
                    AllNodes.Add(First);
                    Head = AllNodes[0];
                }
                else {
                    bool Handle = false;
                    TLinkedList<UCoreItem*>* LastIter = nullptr;

                    int ItemId = Backpack[Index]->ItemId;
                    auto Iter = Head;
                    while (Iter) {
                        auto IterItemId = (**Iter)->ItemId;
                        if (ItemId != IterItemId) {
                            //????????????id??????????????????????????????
                            if (ItemSortPredicate->Compare(ItemId, IterItemId)) {
                                auto NextIter = Iter->Next();
                                if (!NextIter) {
                                    LastIter = Iter;
                                }
                                Iter = NextIter;
                            }
                            else {
                                //???????????????false?????????????????????
                                TLinkedList<UCoreItem*>* NewNode = new TLinkedList<UCoreItem*>(Backpack[Index]);
                                AllNodes.Add(NewNode);
                                auto NewNodePtr = AllNodes[AllNodes.Num() - 1];
                                auto BeforeNode = Iter->GetPrevLink();
                                NewNodePtr->LinkBefore(Iter);
                                if (Iter == Head) {
                                    //???????????????
                                    Head = NewNodePtr;
                                }
                                Handle = true;
                                break;
                            }
                        }
                        else {
                            //id?????????????????????????????????
                            auto FindItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
                            if (!FindItemInfo) {
                                UE_LOG(GameCore, Error, TEXT("??????id[%d]????????????????????????"), ItemId);
                                //??????
                                for (int NodeIndex = 0; NodeIndex < AllNodes.Num(); ++NodeIndex) {
                                    delete AllNodes[NodeIndex];
                                }
                                return;
                            }
                            int MaxStack = FindItemInfo->MaxStack;
                            if (MaxStack == 0) {
                                MaxStack = INT32_MAX;
                            }
                            int OldNum = (**Iter)->ItemNum;
                            int NewNum = FMath::Min(MaxStack, OldNum + Backpack[Index]->ItemNum);
                            int DiffNum = NewNum - OldNum;
                            if (DiffNum > 0) {
                                (**Iter)->ItemNum = NewNum;
                                Backpack[Index]->ItemNum -= DiffNum;
                                if (Backpack[Index]->ItemNum == 0) {
                                    Backpack[Index] = nullptr;
                                    Handle = true;
                                    break;
                                }
                            }
                            //?????????????????????????????????????????????????????????
                            TLinkedList<UCoreItem*>* NewNode = new TLinkedList<UCoreItem*>(Backpack[Index]);
                            AllNodes.Add(NewNode);
                            auto NewNodePtr = AllNodes[AllNodes.Num() - 1];
                            NewNodePtr->LinkBefore(Iter);
                            if (Iter == Head) {
                                //???????????????
                                Head = NewNodePtr;
                            }
                            Handle = true;
                            break;
                        }
                    }
                    if (!Handle) {
                        //??????????????????????????????????????????????????????
                        TLinkedList<UCoreItem*>* NewNode = new TLinkedList<UCoreItem*>(Backpack[Index]);
                        AllNodes.Add(NewNode);
                        auto NewNodePtr = AllNodes[AllNodes.Num() - 1];
                        NewNodePtr->LinkAfter(LastIter);
                    }
                }
            }
        }

        //???????????????????????????????????????????????????????????????????????????
        if (Head) {
            //?????????????????????
            for (int Index = 0; Index < Backpack.Num(); ++Index) {
                Backpack[Index] = nullptr;
            }
            //?????????????????????????????????????????????
            auto Iter = Head;
            for (int Index = AllNodes.Num() - 1; Index >= 0; --Index) {
                Backpack[Index] = **Iter;
                Iter = Iter->Next();
            }
            //??????
            for (int NodeIndex = 0; NodeIndex < AllNodes.Num(); ++NodeIndex) {
                delete AllNodes[NodeIndex];
            }
            BackpackComponent->NotifyBackpackChanged();
        }
    }
}

int32 UAssetSystem::ChangeMoney(UWalletComponent* WalletComponent, uint8 MoneyType, int32 Count, bool bConsume, const FString& Reason, FString& Error) {
	int32 ChangeResult = ChangeMoneyPrivate(WalletComponent, MoneyType, Count, bConsume, Reason, Error);
	return ChangeResult;
}

bool UAssetSystem::CanDeductMoney(UWalletComponent* WalletComponent, const TArray<FMoneyTypeNumPair>& DeductMoneys) {
    TMap<uint32, int32> MoneyTotal;
    for (auto MoneyInfo : WalletComponent->Wallets) {
        MoneyTotal.Add(MoneyInfo.MoneyType, MoneyInfo.Count);
    }
    for (auto MoneyTypeNumPair : DeductMoneys) {
        if (!MoneyTotal.Contains(MoneyTypeNumPair.MoneyType)) {
            return false;
        }
        MoneyTotal[MoneyTypeNumPair.MoneyType] -= MoneyTypeNumPair.Num;
        if (MoneyTotal[MoneyTypeNumPair.MoneyType] < 0) {
            return false;
        }
    }
    return true;
}

void UAssetSystem::ChangeMoney(UWalletComponent* WalletComponent, const TArray<FMoneyTypeNumPair>& ChangeMoneys, bool bConsume, const FString& Reason, FString& Error) {
    for (auto MoneyTypeNumPair : ChangeMoneys) {
        ChangeMoney(WalletComponent, MoneyTypeNumPair.MoneyType, MoneyTypeNumPair.Num, bConsume, Reason, Error);
    }
}

FAssetBackpack& UAssetSystem::MakePackageExist(UBackpackComponent* BackpackComponent, uint8 BackpackType) {
    return BackpackComponent->FindOrAddPackage(BackpackType);
}

UCoreItem* UAssetSystem::GenerateNewItem(UBackpackComponent* BackpackComponent, int ItemId, UClass* ItemClass) {
    auto AddItem = NewObject<UCoreItem>(BackpackComponent, ItemClass);
    AddItem->ItemId = ItemId;

    return AddItem;
}

TMap<int32, int32> UAssetSystem::SimulateAddItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot, bool Force, const FString& Reason, FString& Error) {
    TMap<int32, int32> TempChangeItems;
	if (Count == 0) {
		Error = TEXT("?????????????????????0");
		return TempChangeItems;
	}
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
    if (BackpackType == FBackpackTypeConfigTableRow::BackpackTypeMax) {
        //??????????????????
        auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
        if (ItemInfo) {
            auto ItemTypeInfo = (FItemTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemTypeDataTable, ItemInfo->ItemType);
            if (ItemTypeInfo) {
                BackpackType = ItemTypeInfo->DefaultBackpackType;
            }
        }
    }
    auto BackpackExtendHandler = GetBackpackExtendHandler();
    if (!BackpackExtendHandler->AllowItemAdd(BackpackComponent, ItemId, BackpackType)) {
        Error = TEXT("?????????????????????????????????");
        return TempChangeItems;
    }

	auto& Package = MakePackageExist(BackpackComponent, BackpackType);
    
	auto AddItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
	if (!AddItemInfo) {
		Error = TEXT("????????????Id?????????");
		return TempChangeItems;
	}
    int MaxStack = AddItemInfo->MaxStack;
    if (MaxStack == 0) {
        MaxStack = INT32_MAX;
    }
	auto& Backpack = Package.ItemList;
    if (SpecialSlot == -1) {
        //????????????????????????
        //????????????????????????????????????
        TArray<int32> ItemIndexArray;
        for (int Index = 0; Index != Backpack.Num(); ++Index) {
            if (Backpack[Index] && Backpack[Index]->ItemId == ItemId) {
                ItemIndexArray.Add(Index);
                TempChangeItems.Add(Index, Backpack[Index]->ItemNum);
            }
        }
        int RestCount = Count;
        for (int Index = 0; Index < ItemIndexArray.Num(); ++Index) {
            int OldCount = TempChangeItems[ItemIndexArray[Index]];
            int NewCount = FMath::Min(MaxStack, OldCount + RestCount);
            int ChangeCount = NewCount - OldCount;
            if (ChangeCount > 0) {
                TempChangeItems[ItemIndexArray[Index]] = NewCount;
                RestCount -= ChangeCount;
                if (RestCount == 0) {
                    break;
                }
            }
        }
        if (RestCount > 0) {
            //???????????????????????????????????????????????????
            //????????????
            for (int Index = 0; Index != Backpack.Num(); ++Index) {
                if (Backpack[Index] == nullptr) {
                    int ChangeCount = FMath::Min(MaxStack, RestCount);
                    TempChangeItems.Add(Index, ChangeCount);
                    RestCount -= ChangeCount;
                    if (RestCount == 0) {
                        break;
                    }
                }
            }
        }
        if (RestCount > 0) {
            //???????????????????????????????????????????????????
            if (Force) {
                //?????????????????????????????????????????????????????????
            }
            else {
                Error = TEXT("????????????");
            }
            return TempChangeItems;
        }
    }
    else {
        //?????????????????????
        if (SpecialSlot < 0 || SpecialSlot >= Backpack.Num()) {
            Error = TEXT("??????????????????");
            return TempChangeItems;
        }
        int OldNum = 0;
        if (Backpack[SpecialSlot]) {
            OldNum = Backpack[SpecialSlot]->ItemNum;
        }
        int NewNum = OldNum + Count;
        if (NewNum > MaxStack) {
            Error = TEXT("????????????");
            return TempChangeItems;
        }
        TempChangeItems.Add(SpecialSlot, NewNum);
    }
    return TempChangeItems;
}

TMap<int32, TMap<int32, TArray<TPair<int32, int32>>>> UAssetSystem::SimulateAddItems(UBackpackComponent* BackpackComponent, const TArray<FAddItemInfo>& AddItems, bool Force, const FString& Reason, FString& Error) {
    TMap<int32, TMap<int32, TArray<TPair<int32, int32>>>> TempChangeItems;
    if (AddItems.Num() == 0) {
        Error = TEXT("?????????????????????0");
        return TempChangeItems;
    }
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemTypeDataTable = ItemSetting->ItemTypeTable.LoadSynchronous();
    auto BackpackExtendHandler = GetBackpackExtendHandler();
    //?????????key???packagetype????????????key???itemid???value???AddItems?????????
    TMap<int, TMap<int, int>> TotalItems;
    for (int Index = 0; Index < AddItems.Num(); ++Index) {
        int BackpackType = AddItems[Index].BackpackType;
        if (BackpackType == FBackpackTypeConfigTableRow::BackpackTypeMax) {
            //??????????????????
            auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, AddItems[Index].ItemId);
            if (ItemInfo) {
                auto ItemTypeInfo = (FItemTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemTypeDataTable, ItemInfo->ItemType);
                if (ItemTypeInfo) {
                    BackpackType = ItemTypeInfo->DefaultBackpackType;
                }
            }
        }
        if (!BackpackExtendHandler->AllowItemAdd(BackpackComponent, AddItems[Index].ItemId, BackpackType)) {
            Error = TEXT("?????????????????????????????????");
            return TempChangeItems;
        }

        TMap<int, int>& ItemArray = TotalItems.FindOrAdd(BackpackType);
        ItemArray.Add(AddItems[Index].ItemId, Index);
    }
    for (auto PackageIter = TotalItems.CreateConstIterator(); PackageIter; ++PackageIter) {
        auto BackpackType = PackageIter->Key;
        auto& Package = MakePackageExist(BackpackComponent, BackpackType);
        auto& Backpack = Package.ItemList;
        //????????????
        TMap<int32, TArray<TPair<int32, int32>>>& ChangeItems = TempChangeItems.FindOrAdd(BackpackType);
        for (int Index = 0; Index < Backpack.Num(); ++Index) {
            if (Backpack[Index] && PackageIter->Value.Contains(Backpack[Index]->ItemId)) {
                //????????????itemid
                TArray<TPair<int32, int32>>& ItemIndexArray = ChangeItems.FindOrAdd(Backpack[Index]->ItemId);
                ItemIndexArray.Add(TPair<int32, int32>(Index, Backpack[Index]->ItemNum));
            }
        }
        TArray<TPair<int, int>> RestAddItems;
        //????????????????????????????????????????????????????????????
        for (auto AddItemIter = PackageIter->Value.CreateConstIterator(); AddItemIter; ++AddItemIter) {
            auto ItemId = AddItemIter->Key;
            auto AddItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
            if (!AddItemInfo) {
                Error = TEXT("????????????Id?????????");
                return TempChangeItems;
            }
            int MaxStack = AddItemInfo->MaxStack;
            if (MaxStack == 0) {
                MaxStack = INT32_MAX;
            }
            const auto& AddItem = AddItems[AddItemIter->Value];
            int RestCount = AddItem.Count;
            TArray<TPair<int32, int32>>& TempItemInfo = ChangeItems.FindOrAdd(ItemId);
            for (int Index = 0; Index < TempItemInfo.Num(); ++Index) {
                int NewNum = FMath::Min(MaxStack, TempItemInfo[Index].Value + RestCount);
                int DiffNum = NewNum - TempItemInfo[Index].Value;
                if (DiffNum > 0) {
                    TempItemInfo[Index] = TPair<int32, int32>(TempItemInfo[Index].Key, NewNum);
                    RestCount -= DiffNum;
                    if (RestCount == 0) {
                        break;
                    }
                }
            }
            if (RestCount > 0) {
                //?????????????????????
                RestAddItems.Add(TPair<int, int>(ItemId, RestCount));
            }
        }
        //????????????????????????
        if (RestAddItems.Num() > 0) {
            //????????????????????????
            int AddIndex = 0;
            for (int Index = 0; Index < Backpack.Num(); ++Index) {
                if (Backpack[Index] == nullptr) {
                    auto ItemId = RestAddItems[AddIndex].Key;
                    auto AddItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
                    int MaxStack = AddItemInfo->MaxStack;
                    if (MaxStack == 0) {
                        MaxStack = INT32_MAX;
                    }
                    int NewNum = FMath::Min(MaxStack, RestAddItems[AddIndex].Value);
                    TArray<TPair<int, int>>& TempItemIndexArray = ChangeItems.FindOrAdd(ItemId);
                    TempItemIndexArray.Add(TPair<int, int>(Index, NewNum));
                    int RestCount = RestAddItems[AddIndex].Value - NewNum;
                    if (RestCount > 0) {
                        RestAddItems[AddIndex] = TPair<int, int>(ItemId, RestCount);
                    }
                    else {
                        ++AddIndex;
                        if (AddIndex == RestAddItems.Num()) {
                            break;
                        }
                    }
                }
            }
            if (AddIndex < RestAddItems.Num()) {
                //todo, ????????????????????????????????????????????????????????????????????????
                if (Force) {

                }
                else {
                    Error = TEXT("????????????");
                }
            }
        }
    }

    return TempChangeItems;
}

bool UAssetSystem::UseItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason) {
	auto& Package = BackpackComponent->GetBackpack(BackpackType);
	if (!UAssetBackpackBlueprintLibrary::IsValid(Package)) {
        return false;
    }
	auto& Backpack = Package.ItemList;
	if (SlotIndex >= Backpack.Num()) {
		return false;
	}
	auto BackpackItem = Backpack[SlotIndex];

	auto ItemId = BackpackItem->ItemId;
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();

    auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
	if (!ItemInfo) {
		return false;
	}
	if (ItemInfo->ItemId != ItemId) {
		return false;
	}

	SendUseItemEvent(BackpackComponent, ItemId, Count);

	return true;
}

UCoreItem* UAssetSystem::AbandonItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason, FString& Error) {
	auto& Package = BackpackComponent->GetBackpack(BackpackType);
	if (!UAssetBackpackBlueprintLibrary::IsValid(Package)) {
		return nullptr;
	}
	auto& Backpack = Package.ItemList;

	if (SlotIndex >= Backpack.Num()) {
		Error = TEXT("???????????????????????????????????????");
		//????????????
		return nullptr;
	}
	if (Backpack[SlotIndex] == nullptr) {
		Error = TEXT("????????????????????????");
		return nullptr;
	}
	int ItemId = Backpack[SlotIndex]->ItemId;
	if (Count == -1) {
		Count = Backpack[SlotIndex]->ItemNum;
	}
	UCoreItem* Result = nullptr;;
	auto AbandonResult = ReduceItem(BackpackComponent, BackpackType, SlotIndex, Count, Reason, Error);
	if (AbandonResult) {
        const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
        auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
		auto ItemClass = UCoreItem::StaticClass();
        auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
		if (!ItemInfo) {
			Error = TEXT("??????????????????");
			return nullptr;
		}
		if (ItemInfo->ItemClass) {
			ItemClass = ItemInfo->ItemClass;
		}
        Result = NewObject<UCoreItem>(BackpackComponent, ItemClass);
        Result->ItemId = ItemId;
		Result->ItemNum = Count;
	}
	return Result;
}

TMap<int, TMap<int32, int32>> UAssetSystem::SimulateDeductItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int Count, int32 SpecialSlot, const FString& Reason, FString& Error) {
    TMap<int, TMap<int32, int32>> TempChangeItems;
    int RestCount = Count;
    if (SpecialSlot == -1) {
        for (auto Index = 0; Index < BackpackComponent->Backpacks.Num(); ++Index) {
            auto& Package = BackpackComponent->Backpacks[Index];
            auto BackpackTypeId = Package.BackpackType;
            if (BackpackType == -1 || BackpackType == BackpackTypeId) {
                auto& Backpack = Package.ItemList;

                for (int32 SlotIndex = 0; SlotIndex < Backpack.Num(); ++SlotIndex) {
                    if (Backpack[SlotIndex]) {
                        if (Backpack[SlotIndex]->ItemId == ItemId) {
                            TMap<int32, int32>& TempItems = TempChangeItems.FindOrAdd(BackpackType);
                            int RemoveCount = FMath::Min(Backpack[SlotIndex]->ItemNum, RestCount);
                            TempItems.Add(SlotIndex, RemoveCount);
                            RestCount -= RemoveCount;
                            if (RestCount == 0) {
                                break;
                            }
                        }
                    }
                }
                if (RestCount == 0) {
                    break;
                }
            }
        }
    }
    else {
        auto& Package = BackpackComponent->GetBackpack(BackpackType);
        if (UAssetBackpackBlueprintLibrary::IsValid(Package)) {
            if (SpecialSlot >= 0 && SpecialSlot < Package.ItemList.Num()) {
                if (Package.ItemList[SpecialSlot] && Package.ItemList[SpecialSlot]->ItemNum > Count) {
                    TMap<int32, int32>& TempItems = TempChangeItems.FindOrAdd(BackpackType);
                    TempItems.Add(SpecialSlot, Count);
                }
            }
        }
    }

    if (RestCount > 0) {
        Error = TEXT("??????????????????");
    }

    return TempChangeItems;
}

TMap<int32, TMap<int32, TArray<TPair<int32, int32>>>> UAssetSystem::SimulateDeductItems(UBackpackComponent* BackpackComponent, const TArray<FItemIDNumPair>& DeductItems, const FString& Reason, FString& Error) {
    TMap<int32, TMap<int32, TArray<TPair<int32, int32>>>> TempChangeItems;
    if (DeductItems.Num() == 0) {
        Error = TEXT("?????????????????????0");
        return TempChangeItems;
    }
    
    TMap<int, int> TotalRemoves;
    for (int Index = 0; Index < DeductItems.Num(); ++Index) {
        if (TotalRemoves.Contains(DeductItems[Index].ItemID)) {
            TotalRemoves[DeductItems[Index].ItemID] += DeductItems[Index].ItemNum;
        }
        else {
            TotalRemoves.Add(DeductItems[Index].ItemID, DeductItems[Index].ItemNum);
        }
    }

    for (auto PackageIndex = 0; PackageIndex < BackpackComponent->Backpacks.Num(); ++PackageIndex) {
        auto& Package = BackpackComponent->Backpacks[PackageIndex];
        auto BackpackTypeId = Package.BackpackType;
        auto& Backpack = Package.ItemList;
        for (int ItemIndex = 0; ItemIndex < Backpack.Num(); ++ItemIndex) {
            if (Backpack[ItemIndex]) {
                auto ItemId = Backpack[ItemIndex]->ItemId;
                if (TotalRemoves.Contains(ItemId)) {
                    int RemoveCount = FMath::Min(Backpack[ItemIndex]->ItemNum, TotalRemoves[ItemId]);
                    TotalRemoves[ItemId] -= RemoveCount;
                    TempChangeItems.FindOrAdd(BackpackTypeId).FindOrAdd(ItemId).Add(TPair<int32, int32>(ItemIndex, RemoveCount));
                    if (TotalRemoves[ItemId] == 0) {
                        TotalRemoves.Remove(ItemId);
                        if (TotalRemoves.Num() == 0) {
                            break;
                        }
                    }
                }
            }
        }
        if (TotalRemoves.Num() == 0) {
            break;
        }
    }

    if (TotalRemoves.Num() > 0) {
        Error = TEXT("?????????????????????0");
    }

    return TempChangeItems;
}

bool UAssetSystem::MoveItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int32 NewPackageType, int NewSlotIndex, FString& Error) {
	auto& Package = BackpackComponent->GetBackpack(BackpackType);
	if (!UAssetBackpackBlueprintLibrary::IsValid(Package)) {
		return false;
	}

	auto& Backpack = Package.ItemList;

	auto& NewPackage = MakePackageExist(BackpackComponent, NewPackageType);

	auto& NewBackpack = NewPackage.ItemList;

    //newSlotIndex??????-1?????????????????????????????????????????????
    if (NewSlotIndex == -1) {
        for (int Index = 0; Index < NewBackpack.Num(); ++Index) {
            if (NewBackpack[Index] == nullptr) {
                NewSlotIndex = Index;
                break;
            }
        }
    }

	if (SlotIndex < 0 || SlotIndex >= Backpack.Num() || NewSlotIndex < 0 || NewSlotIndex >= NewBackpack.Num()) {
		//????????????
		Error = TEXT("????????????");
		return false;
	}
    
	auto BackpackItem = Backpack[SlotIndex];
	if (BackpackItem == nullptr) {
		//?????????????????????????????????
		Error = TEXT("?????????????????????");
		return false;
	}

	auto NewSlotIndexOrigineItem = NewBackpack[NewSlotIndex];

    auto BackpackExtendHandler = GetBackpackExtendHandler();
    if (!BackpackExtendHandler->CanMoveItem(BackpackComponent, BackpackType, SlotIndex, BackpackItem, NewPackageType, NewSlotIndex, NewSlotIndexOrigineItem)) {
        Error = TEXT("???????????????????????????");
        return false;
    }

	if (NewSlotIndexOrigineItem != nullptr) {
		//???????????????????????????????????????????????????????????????
		if (NewSlotIndexOrigineItem->ItemId == BackpackItem->ItemId) {
            const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
            auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();

            auto FindItem = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, BackpackItem->ItemId);
            if (!FindItem) {
                Error = TEXT("????????????????????????id");
                return false;
            }
            int MaxStack = FindItem->MaxStack;
            if (MaxStack == 0) {
                MaxStack = INT32_MAX;
            }
            auto NewNum = FMath::Min(MaxStack, NewSlotIndexOrigineItem->ItemNum + BackpackItem->ItemNum);
            auto DiffNum = NewNum - NewSlotIndexOrigineItem->ItemNum;
            if (DiffNum == 0) {
                OnItemLeavePackage(BackpackComponent, Backpack[SlotIndex], BackpackType, SlotIndex);
                OnItemLeavePackage(BackpackComponent, NewBackpack[NewSlotIndex], NewPackageType, NewSlotIndex);

                //?????????????????????????????????
                Backpack[SlotIndex] = NewSlotIndexOrigineItem;
                NewBackpack[NewSlotIndex] = BackpackItem;

                OnItemEnterPackage(BackpackComponent, Backpack[SlotIndex], BackpackType, SlotIndex);
                OnItemEnterPackage(BackpackComponent, NewBackpack[NewSlotIndex], NewPackageType, NewSlotIndex);
            }
            else {
                BackpackItem->ItemNum -= DiffNum;
                NewSlotIndexOrigineItem->ItemNum += DiffNum;

                if (BackpackItem->ItemNum == 0) {
                    //?????????????????????????????????
                    OnItemLeavePackage(BackpackComponent, Backpack[SlotIndex], BackpackType, SlotIndex);
                    Backpack[SlotIndex] = nullptr;
                }
            }
		}
        else {
            OnItemLeavePackage(BackpackComponent, Backpack[SlotIndex], BackpackType, SlotIndex);
            OnItemLeavePackage(BackpackComponent, NewBackpack[NewSlotIndex], NewPackageType, NewSlotIndex);

            //id???????????????
            Backpack[SlotIndex] = NewSlotIndexOrigineItem;
            NewBackpack[NewSlotIndex] = BackpackItem;

            OnItemEnterPackage(BackpackComponent, Backpack[SlotIndex], BackpackType, SlotIndex);
            OnItemEnterPackage(BackpackComponent, NewBackpack[NewSlotIndex], NewPackageType, NewSlotIndex);
        }
	}
	else {
        OnItemLeavePackage(BackpackComponent, Backpack[SlotIndex], BackpackType, SlotIndex);

		Backpack[SlotIndex] = nullptr;
		NewBackpack[NewSlotIndex] = BackpackItem;

        OnItemEnterPackage(BackpackComponent, NewBackpack[NewSlotIndex], NewPackageType, NewSlotIndex);
	}

	return true;
}

int32 UAssetSystem::SplitItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, FString& Error) {
	auto& Package = BackpackComponent->GetBackpack(BackpackType);
	if (!UAssetBackpackBlueprintLibrary::IsValid(Package)) {
		return -1;
	}
	auto& Backpack = Package.ItemList;
	if (SlotIndex >= Backpack.Num()) {
		//????????????
		Error = TEXT("????????????");
		return -1;
	}
	auto BackpackItem = Backpack[SlotIndex];
	if (BackpackItem == nullptr) {
		//?????????????????????????????????
		Error = TEXT("????????????");
		return -1;
	}
	if (BackpackItem->ItemNum <= Count) {
		//????????????????????????????????????????????????
		Error = TEXT("??????????????????");
		return -1;
	}
	auto ItemClass = UCoreItem::StaticClass();
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();

    auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, BackpackItem->ItemId);
	if (!ItemInfo) {
		Error = TEXT("??????????????????");
		return -1;
	}
	if (ItemInfo->ItemClass)
	{
		ItemClass = ItemInfo->ItemClass;
	}

	//????????????
	for (int Index = 0; Index != Backpack.Num(); ++Index) {
		if (Backpack[Index] == nullptr) {
			//?????????????????????

			auto NewItem = GenerateNewItem(BackpackComponent, BackpackItem->ItemId, ItemClass);
			NewItem->ItemNum = Count;

            Backpack[Index] = NewItem;

            OnItemEnterPackage(BackpackComponent, NewItem, BackpackType, Index);

			BackpackItem->ItemNum -= Count;

			return Index;
		}
	}
	//???????????????????????????
	Error = TEXT("??????????????????");
	return -1;
}

int32 UAssetSystem::ChangeMoneyPrivate(UWalletComponent* WalletComponent, uint8 MoneyType, int32 Count, bool bConsume, const FString& Reason, FString& Error) {
    if (Count == 0) {
        Error = TEXT("????????????");
        return -1;
    }
    int FindIndex = -1;
    for (int Index = 0; Index < WalletComponent->Wallets.Num(); ++Index) {
        if (WalletComponent->Wallets[Index].MoneyType == MoneyType) {
            FindIndex = Index;
            break;
        }
    }
    if (Count < 0) {
        if (FindIndex == -1) {
            Error = TEXT("????????????");
            return -1;
        }
        if (WalletComponent->Wallets[FindIndex].Count + Count < 0) {
            Error = TEXT("????????????");
            return -1;
        }
    }
    else {
        if (FindIndex == -1) {
            FMoneyInfo MoneyInfo;
            MoneyInfo.MoneyType = MoneyType;
            MoneyInfo.Count = 0;
            FindIndex = WalletComponent->Wallets.Num();
            WalletComponent->Wallets.Add(MoneyInfo);
        }
    }
    
    UE_LOG(GameCore, Display, TEXT("?????????????????????[%d],??????[%d],????????????[%d]???Reason:[%s]"), MoneyType, Count, WalletComponent->Wallets[FindIndex].Count + Count, *Reason);
    WalletComponent->Wallets[FindIndex].Count = WalletComponent->Wallets[FindIndex].Count + Count;

    SendChangeMoneyEvent(WalletComponent, MoneyType, Count);
    if (bConsume && Count < 0) {
        SendUseMoneyEvent(WalletComponent, MoneyType, -Count);
    }

    return WalletComponent->Wallets[FindIndex].Count;
}

UCoreItem* UAssetSystem::ReduceItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason, FString& Error) {
	auto& Package = BackpackComponent->GetBackpack(BackpackType);
	if (!UAssetBackpackBlueprintLibrary::IsValid(Package)) {
		Error = TEXT("??????????????????");
		return nullptr;
	}
	auto& Backpack = Package.ItemList;
	auto BackpackItem = Backpack[SlotIndex];
	if (BackpackItem->ItemNum < Count) {
		Error = TEXT("??????????????????");
		//????????????
		return nullptr;
	}

	BackpackItem->ItemNum -= Count;
	if (BackpackItem->ItemNum == 0) {
        OnItemLeavePackage(BackpackComponent, BackpackItem, BackpackType, SlotIndex);
		Backpack[SlotIndex] = nullptr;
	}
	UE_LOG(GameCore, Display, TEXT("??????????????????[%d]???itemid???[%d]?????????[%d]???????????????[%d] Reason:[%s]"), BackpackType, BackpackItem->ItemId, Count, BackpackItem->ItemNum, *Reason);

	SendChangeItemEvent(BackpackComponent, BackpackItem->ItemId, -Count, Reason);

	return BackpackItem;
}

void UAssetSystem::SendChangeItemEvent(UBackpackComponent* BackpackComponent, int32 ItemId, int32 Count, const FString& Reason) {
	UChangeItemEvent* ChangeItemEvent = NewObject<UChangeItemEvent>();
    ChangeItemEvent->Source = BackpackComponent->GetOwner();
    ChangeItemEvent->ItemId = ItemId;
    ChangeItemEvent->Count = Count;
    ChangeItemEvent->Reason = Reason;
	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(ChangeItemEvent);
}

void UAssetSystem::SendUseItemEvent(UBackpackComponent* BackpackComponent, int32 ItemId, int32 Count) {
	UConsumeItemEvent* ConsumeItemEvent = NewObject<UConsumeItemEvent>();
    ConsumeItemEvent->Source = BackpackComponent->GetOwner();
    ConsumeItemEvent->ItemID = ItemId;
    ConsumeItemEvent->Count = Count;
	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(ConsumeItemEvent);
}

void UAssetSystem::SendChangeMoneyEvent(UWalletComponent* WalletComponent, uint8 MoneyType, int32 moneyCount) {
	UChangeMoneyEvent* changeMoneyEvent = NewObject<UChangeMoneyEvent>();
	changeMoneyEvent->Source = WalletComponent->GetOwner();
	changeMoneyEvent->MoneyType = MoneyType;
	changeMoneyEvent->MoneyCount = moneyCount;
	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(changeMoneyEvent);
}

void UAssetSystem::SendUseMoneyEvent(UWalletComponent* WalletComponent, uint8 MoneyType, int32 moneyCount) {
	UConsumeMoneyEvent* consumeMoneyEvent = NewObject<UConsumeMoneyEvent>();
	consumeMoneyEvent->Source = WalletComponent->GetOwner();
	consumeMoneyEvent->MoneyType = MoneyType;
	consumeMoneyEvent->MoneyCount = moneyCount;
	GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(consumeMoneyEvent);
}

class UBackpackExtendHandler* UAssetSystem::GetBackpackExtendHandler() {
    TSubclassOf<UBackpackExtendHandler> BackpackExtendHandlerClass = UBackpackExtendHandler::StaticClass();
    const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
    FString BackpackExtendHandlerClassPath = BackpackSetting->BackpackExtendHandlerClass.ToString();
    if (!BackpackExtendHandlerClassPath.IsEmpty()) {
        TSubclassOf<UBackpackExtendHandler> LoadClass = StaticLoadClass(UBackpackExtendHandler::StaticClass(), NULL, *BackpackExtendHandlerClassPath);
        if (LoadClass) {
            BackpackExtendHandlerClass = LoadClass;
        }
    }
    return Cast<UBackpackExtendHandler>(BackpackExtendHandlerClass->GetDefaultObject());
}

void UAssetSystem::OnItemEnterPackage(UBackpackComponent* BackpackComponent, class UCoreItem* Item, uint8 BackpackType, int Index) {
    if (BackpackComponent->GetOwner()->GetLocalRole() == ENetRole::ROLE_Authority) {
        //??????????????????????????????
        GetBackpackExtendHandler()->OnItemAdd(BackpackComponent, Item, BackpackType, Index);
    }
}

void UAssetSystem::OnItemLeavePackage(UBackpackComponent* BackpackComponent, class UCoreItem* Item, uint8 BackpackType, int Index) {
    if (BackpackComponent->GetOwner()->GetLocalRole() == ENetRole::ROLE_Authority) {
        //??????????????????????????????
        GetBackpackExtendHandler()->OnItemRemove(BackpackComponent, Item, BackpackType, Index);
    }
}

TArray<UClass*> UAssetSystem::GetHandleEventTypes_Implementation() {
	return TArray<UClass*>({
		UAddItemRequesEvent::StaticClass(),
        UAddItemsRequesEvent::StaticClass(),
		UUseItemRequesEvent::StaticClass(),
		UAbandonItemRequesEvent::StaticClass(),
		UDeductItemRequesEvent::StaticClass(),
        UDeductItemsRequesEvent::StaticClass(),
		UMoveItemRequesEvent::StaticClass(),
		USplitItemRequesEvent::StaticClass(),
		UPickupItemRequesEvent::StaticClass(),
        USortBackpackRequestEvent::StaticClass(),
        UChangeMoneyRequestEvent::StaticClass(),
	});
}

void UAssetSystem::OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	if (HandleEvent->IsA(UAddItemRequesEvent::StaticClass())) {
		auto Request = Cast<UAddItemRequesEvent>(HandleEvent);

		auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
		if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
			if (CharacterState && CharacterState->BackpackComponent) {
				FString Error;
				AddItem(CharacterState->BackpackComponent, Request->BackpackType, Request->ItemId, Request->Count, Request->SpecialSlot, false, Request->Reason, Error);
			}
		}
	}
    if (HandleEvent->IsA(UAddItemsRequesEvent::StaticClass())) {
        auto Request = Cast<UAddItemsRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
                FString Error;
                AddItems(CharacterState->BackpackComponent, Request->AddItemInfos, false, Request->Reason, Error);
            }
        }
    }
	else if (HandleEvent->IsA(UUseItemRequesEvent::StaticClass())) {
		auto Request = Cast<UUseItemRequesEvent>(HandleEvent);

		auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
		if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
				UseItem(CharacterState->BackpackComponent, Request->BackpackType, Request->SlotIndex, Request->Count, Request->Reason);
			}
		}
	}
	else if (HandleEvent->IsA(UAbandonItemRequesEvent::StaticClass())) {
		auto Request = Cast<UAbandonItemRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
				FString Error;
				auto AbandonItemResult = AbandonItem(CharacterState->BackpackComponent, Request->BackpackType, Request->SlotIndex, Request->Count, Request->Reason, Error);
				if (AbandonItemResult) {
					FActorSpawnParameters Paramters;
					Paramters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                    auto Transform = Entity->GetTransform();
                    auto Location = Transform.GetLocation();
                    auto Rotator = Transform.GetRotation().Rotator();
					auto SceneItemClass = StaticLoadClass(ACoreSceneItem::StaticClass(), NULL, TEXT("Blueprint'/GameFramework/SceneItem.SceneItem_C'"));
					auto SpawnSceneItem = GetWorld()->SpawnActor<ACoreSceneItem>(SceneItemClass, Paramters);
					SpawnSceneItem->UpdateItem(AbandonItemResult->ItemId, AbandonItemResult->ItemNum);
                    SpawnSceneItem->SetActorLocationAndRotation(Location, Rotator);
				}
			}
		}
	}
	else if (HandleEvent->IsA(UDeductItemRequesEvent::StaticClass())) {
		auto Request = Cast<UDeductItemRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
				FString Error;
				DeductItem(CharacterState->BackpackComponent, Request->BackpackType, Request->ItemId, Request->Count, Request->SpecialSlot, Request->Reason, Error);
			}
		}
	}
	else if (HandleEvent->IsA(UMoveItemRequesEvent::StaticClass())) {
		auto Request = Cast<UMoveItemRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
				FString Error;
				MoveItem(CharacterState->BackpackComponent, Request->BackpackType, Request->SlotIndex, Request->NewPackageType, Request->NewSlotIndex, Error);
			}
		}
	}
	else if (HandleEvent->IsA(USplitItemRequesEvent::StaticClass())) {
		auto Request = Cast<USplitItemRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
				FString Error;
				SplitItem(CharacterState->BackpackComponent, Request->BackpackType, Request->SlotIndex, Request->Count, Error);
			}
		}
	}
	else if (HandleEvent->IsA(UPickupItemRequesEvent::StaticClass())) {
		auto Request = Cast<UPickupItemRequesEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
				auto DropItem = Cast<ACoreSceneItem>(InGameInstance->GameEntityManager->GetEntityById(Request->DropEntityId).GetObject());
				if (!DropItem) {
					return;
				}

				if (DropItem->HasAnyFlags(RF_BeginDestroyed)) return;
				int RealPickupCount = Request->Count;
				if (RealPickupCount <= 0 || RealPickupCount > DropItem->ItemComponent->ItemCount)
					RealPickupCount = DropItem->ItemComponent->ItemCount;
				FString Error;
				int AddCount = InGameInstance->GameSystemManager->GetSystemByClass<UAssetSystem>()->AddItem(CharacterState->BackpackComponent, Request->BackpackType, DropItem->ItemComponent->ItemId, RealPickupCount, -1, false, TEXT("Pickup"), Error);
				if (AddCount == DropItem->ItemComponent->ItemCount) {
					DropItem->Destroy();
				}
				else {
					DropItem->ItemComponent -= AddCount;
				}
			}
		}
	}
    else if (HandleEvent->IsA(USortBackpackRequestEvent::StaticClass())) {
        auto Request = Cast<USortBackpackRequestEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
            if (CharacterState && CharacterState->BackpackComponent) {
                SortBackpack(CharacterState->BackpackComponent, Request->BackpackType);
            }
        }
    }
    else if (HandleEvent->IsA(UChangeMoneyRequestEvent::StaticClass())) {
        auto Request = Cast<UChangeMoneyRequestEvent>(HandleEvent);

        auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
        if (Entity) {
            auto PlayerState = UGameFrameworkUtils::GetEntityState(Entity);
            if (PlayerState && PlayerState->WalletComponent) {
                FString Error;
                ChangeMoney(PlayerState->WalletComponent, Request->MoneyType, Request->Count, Request->bConsume, Request->Reason, Error);
            }
        }
    }
}