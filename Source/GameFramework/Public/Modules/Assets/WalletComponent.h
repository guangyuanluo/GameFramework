// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Modules/Events/GameEventBase.h"
#include "WalletComponent.generated.h"

/**
 * 货币数据
 */
USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FMoneyInfo {
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Wallet")
    uint8 MoneyType;

    UPROPERTY(BlueprintReadOnly, Category = "Wallet")
    int32 Count;
};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UWalletComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWalletComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	UPROPERTY(ReplicatedUsing = OnWalletRefresh, Category = "Wallet", EditAnywhere, BlueprintReadOnly)
	TArray<FMoneyInfo> Wallets;					///< 钱包

	/**
	* @brief 改钱
	* @param moneyType 货币类型
	* @param count 货币数量
	* @param bUse 是否是花费了钱
	* @param reason 改钱原因
	*/
	UFUNCTION(BlueprintCallable, Category = "Wallet")
	void ChangeMoney(uint8 MoneyType, int32 Count, bool bConsume, const FString& Reason);

    /**
    * @brief 获取货币数量
    */
    UFUNCTION(BlueprintCallable, Category = "Wallet")
    int GetMoneyCount(uint8 MoneyType);

    UFUNCTION()
	void OnWalletRefresh();
};

/*************事件定义**************/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOnWalletRefreshEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        UWalletComponent* WalletComponent;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UChangeMoneyRequestEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString EntityId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        uint8 MoneyType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        int32 Count;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        bool bConsume;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
        FString Reason;
};