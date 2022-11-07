// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityComponent.h"
#include "StoreComponent.generated.h"

class ACoreCharacter;

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FStoreGoodsInfo {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Store")
	int32 GoodsID;
};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UStoreComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStoreComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Store")
	int32 StoreID;

	UPROPERTY(ReplicatedUsing = OnGoodsChanged, Category = "Store", EditAnywhere, BlueprintReadOnly)
	TArray<FStoreGoodsInfo> GoodsInfos;		///< 商品信息

	/**
	* 购买商品
	*/
	UFUNCTION(BlueprintCallable, Category = "Backpack")
	void BuyGoods(ACoreCharacter* Character, int32 GoodsID);

private:
	UFUNCTION()
	void OnGoodsChanged();
};