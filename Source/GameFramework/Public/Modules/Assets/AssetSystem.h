// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "Base/ECS/SystemBase.h"
#include "Modules/Assets/AssetBackpack.h"
#include "Modules/Assets/BackpackComponent.h"
#include "Modules/Item/ItemIDNumPair.h"
#include "Modules/Money/MoneyTypeNumPair.h"
#include "AssetSystem.generated.h"

class UWalletComponent;
class UCoreGameInstance;
class UCoreItem;

/*
* @brief 资产系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAssetSystem : public USystemBase, public IEventHandlerInterface {
public:
	GENERATED_BODY()

	virtual void Initialize(UCoreGameInstance* InGameInstance) override;
	virtual void Uninitialize() override;

	/**
	* 增加一个物品
	*/
	int32 AddItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot, bool Force, const FString& Reason, FString& Error);
    /**
    * 能否添加物品
    */
    bool CanAddItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot);
    /**
    * 批量增加物品
    */
    bool AddItems(UBackpackComponent* BackpackComponent, const TArray<FAddItemInfo>& AddItems, bool Force, const FString& Reason, FString& Error);
    /**
    * 能否批量添加物品
    */
    bool CanAddItems(UBackpackComponent* BackpackComponent, const TArray<FAddItemInfo>& AddItems);

	/**
	* 使用物品
	*/
	bool UseItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason);

	/**
	* 丢弃物品
	*/
	UCoreItem* AbandonItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason, FString& Error);
	/**
	* 扣除物品
	*/
	bool DeductItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int Count, int32 SpecialSlot, const FString& Reason, FString& Error);
    /**
    * 能否扣除物品
    */
    bool CanDeductItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int Count, int32 SpecialSlot);
    /**
    * 批量扣除物品
    */
    bool DeductItems(UBackpackComponent* BackpackComponent, const TArray<FItemIDNumPair>& DeductItems, const FString& Reason, FString& Error);
    /**
    * 能否批量扣除物品
    */
    bool CanDeductItems(UBackpackComponent* BackpackComponent, const TArray<FItemIDNumPair>& DeductItems);

	/**
	* 移动背包中某个槽的物品
	*/
	bool MoveItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int32 NewPackageType, int NewSlotIndex, FString& Error);
	/**
	* 拆分背包中某个槽的物品
	*/
	int32 SplitItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, FString& Error);

    /**
    * 整理背包
    */
    void SortBackpack(UBackpackComponent* BackpackComponent, uint8 BackpackType);

	/**
	* 改钱
	*/
	int32 ChangeMoney(UWalletComponent* WalletComponent, uint8 MoneyType, int32 Count, bool bConsume, const FString& Reason, FString& Error);

	/**
	* 能否扣钱
	*/
	bool CanDeductMoney(UWalletComponent* WalletComponent, const TArray<FMoneyTypeNumPair>& DeductMoneys);

	/**
	* 批量改钱
	*/
	void ChangeMoney(UWalletComponent* WalletComponent, const TArray<FMoneyTypeNumPair>& ChangeMoneys, bool bConsume, const FString& Reason, FString& Error);

	/**
	 * 生成一个物品
	 */
	UCoreItem* GenerateNewItem(UBackpackComponent* BackpackComponent, int ItemId, UClass* ItemClass);

private:
	FAssetBackpack& MakePackageExist(UBackpackComponent* BackpackComponent, uint8 BackpackType);
	TMap<int32, int32> SimulateAddItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot, bool Force, const FString& Reason, FString& Error);
    TMap<int32, TMap<int32, TArray<TPair<int32, int32>>>> SimulateAddItems(UBackpackComponent* BackpackComponent, const TArray<FAddItemInfo>& AddItems, bool Force, const FString& Reason, FString& Error);
	bool UseItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason);
	UCoreItem* AbandonItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason, FString& Error);
    TMap<int, TMap<int32, int32>> SimulateDeductItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int32 ItemId, int Count, int32 SpecialSlot, const FString& Reason, FString& Error);
    TMap<int32, TMap<int32, TArray<TPair<int32, int32>>>> SimulateDeductItems(UBackpackComponent* BackpackComponent, const TArray<FItemIDNumPair>& DeductItems, const FString& Reason, FString& Error);
	bool MoveItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int32 NewPackageType, int NewSlotIndex, FString& Error);
	int32 SplitItemPrivate(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, FString& Error);
	int32 ChangeMoneyPrivate(UWalletComponent* WalletComponent, uint8 MoneyType, int32 Count, bool bConsume, const FString& Reason, FString& Error);
	/**
	* 减少背包中某个槽的物品
	*/
	UCoreItem* ReduceItem(UBackpackComponent* BackpackComponent, uint8 BackpackType, int SlotIndex, int Count, const FString& Reason, FString& Error);
	void SendChangeItemEvent(UBackpackComponent* BackpackComponent, int32 ItemId, int32 Count, const FString& Reason);
	void SendUseItemEvent(UBackpackComponent* BackpackComponent, int32 ItemId, int32 Count);
	void SendChangeMoneyEvent(UWalletComponent* WalletComponent, uint8 MoneyType, int32 MoneyCount);
	void SendUseMoneyEvent(UWalletComponent* WalletComponent, uint8 MoneyType, int32 MoneyCount);

    /**
    * 获取背包扩展处理类
    */
    class UBackpackExtendHandler* GetBackpackExtendHandler(UBackpackComponent* BackpackComponent);

    void OnPackageItemChange(UBackpackComponent* BackpackComponent, class UCoreItem* NewItem, class UCoreItem* OldItem, uint8 BackpackType, int Index);

	/** 覆写事件监听 */
	virtual TArray<UClass*> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
};