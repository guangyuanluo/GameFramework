// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/SystemBase.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "StoreSystem.generated.h"

class UCoreGameInstance;
class ACoreCharacter;
class UStoreComponent;

/*
* @brief 商店系统
*/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UStoreSystem : public USystemBase, public IEventHandlerInterface {
public:
	GENERATED_BODY()

	virtual void Initialize(UCoreGameInstance* InGameInstance) override;
	virtual void Uninitialize() override;

	bool BuyGoods(ACoreCharacter* Character, UStoreComponent* StoreComponent, int GoodsID);

private:
	/** 覆写事件监听 */
	virtual TArray<UClass*> GetHandleEventTypes_Implementation() override;
	virtual void OnEvent_Implementation(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
};