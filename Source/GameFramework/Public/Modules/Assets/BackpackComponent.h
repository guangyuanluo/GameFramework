// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityComponent.h"
#include "Modules/Events/GameEventBase.h"
#include "Modules/Assets/AssetBackpack.h"
#include "Modules/Item/ItemIDNumPair.h"
#include "BackpackComponent.generated.h"

extern FAssetBackpack InvalidAssetBackpack;

/**
* 添加物品结构体，用于批量增加物品
*/
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FAddItemInfo {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        uint8 BackpackType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        int32 ItemId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        int32 Count;
};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UBackpackComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBackpackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:	
	UPROPERTY(ReplicatedUsing = OnBackpackChanged, Category = "PlayerInfo", EditAnywhere, BlueprintReadOnly)
	TArray<FAssetBackpack> Backpacks;		///< 所有背包

	/**
	* 增加一个物品
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void AddItem(uint8 BackpackType, int32 ItemId, int32 Count, const FString& Reason, int32 SpecialSlot = -1);

    /**
    * 批量添加物品
    */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    void AddItems(const TArray<FAddItemInfo>& AddItems, const FString& Reason);

	/**
	* @brief 使用物品
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void UseItem(uint8 BackpackType, const FString& InstanceID, int Count, const FString& Reason);
	
	/**
	*  丢弃物品,count为-1表示全部丢弃
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void AbandonItem(uint8 BackpackType, const FString& InstanceID, int Count, const FString& Reason);

	/**
	* 扣除物品，可以指定背包，没有指定背包BackpackType请传-1，这样会找所有背包进行扣除
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void DeductItem(uint8 BackpackType, int32 ItemId, int Count, const FString& Reason, const FString& SpecialInstanceID);

    /**
    * 批量扣除物品
    */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    void DeductItems(const TArray<FItemIDNumPair>& DeductItems, const FString& Reason);

	/**
	* 移动背包中某个槽的物品
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void MoveItem(uint8 BackpackType, const FString& InstanceID, uint8 NewPackageType, int NewSlotIndex = -1);

	/**
	* 拆分背包中某个槽的物品
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void SplitItem(uint8 BackpackType, const FString& InstanceID, int Count);

	/**
	* 拾取物品,pickupCount小于等于0或者大于实际数量，表示全部拾取
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void PickupItem(uint8 BackpackType, class ACoreSceneItem* DropItem, int PickupCount);

    /**
    * 整理背包
    */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    void SortBackpack(uint8 BackpackType);

	/**
	* 得到背包中某个物品的数量
	*/
    UFUNCTION(BlueprintCallable, Category = "Backpack")
	int32 GetBackpackItemCount(int32 ItemId) const;	

    /**
    * 得到某个类型的背包
    */
    UFUNCTION(BlueprintCallable, Category = "Backpack")
    FAssetBackpack& GetBackpack(uint8 BackpackType);

	/**
	* 从指定背包找指定instanceid物品的索引
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	int FindIndexByInstanceID(uint8 BackpackType, const FString& InstanceID);

	/**
	* 通知背包变化
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void NotifyBackpackChanged();

    /**
    * 查找或者添加某个类型的背包
    */
    FAssetBackpack& FindOrAddPackage(uint8 BackpackType);

private:
	UFUNCTION()
	void OnBackpackChanged();
};

/*************事件定义**************/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOnBackpackRefreshEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        class UBackpackComponent* BackpackComponent;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAddItemRequesEvent : public UGameEventBase {
    GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        uint8 BackpackType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        int32 ItemId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        int32 Count;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        int32 SpecialSlot;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString Reason;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAddItemsRequesEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        TArray<FAddItemInfo> AddItemInfos;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString Reason;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UUseItemRequesEvent : public UGameEventBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString EntityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		uint8 BackpackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString InstanceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		int32 Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString Reason;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAbandonItemRequesEvent : public UGameEventBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString EntityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		uint8 BackpackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString InstanceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		int32 Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString Reason;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UDeductItemRequesEvent : public UGameEventBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        uint8 BackpackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		int32 ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		int32 Count;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString SpecialInstanceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString Reason;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UDeductItemsRequesEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        TArray<FItemIDNumPair> DeductItems;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString Reason;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UMoveItemRequesEvent : public UGameEventBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString EntityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		uint8 BackpackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString InstanceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		uint8 NewPackageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		int NewSlotIndex;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API USplitItemRequesEvent : public UGameEventBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString EntityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		uint8 BackpackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString InstanceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		int Count;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UPickupItemRequesEvent : public UGameEventBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString EntityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		uint8 BackpackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		FString DropEntityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
		int Count;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API USortBackpackRequestEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        uint8 BackpackType;
};