// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/SystemBase.h"
#include "Modules/Events/EventHandlerInterface.h"
#include "ExpSystem.generated.h"

class UExpComponent;
/*
* @brief 经验系统
*/
UCLASS()
class GAMEFRAMEWORK_API UExpSystem : public USystemBase, public IEventHandlerInterface {
    GENERATED_BODY()

public:
    virtual void Initialize(UCoreGameInstance* InGameInstance) override;
    virtual void Uninitialize() override;

    /**
    * 增加经验
    */
    bool AddExp(UExpComponent* ExpComponent, uint8 ExpType, int32 Exp, const FString& Reason, FString& Error);

    /**
    * 获取指定等级升级所需经验值
    */
    UFUNCTION(BlueprintCallable, Category = "Utils")
    static int GetLevelUpgradeExp(uint8 ExpType, int32 Level);

private:
    bool AddExpPrivate(class UDataTable* ExpTypeDataTable, UExpComponent* ExpComponent, int ExpIndex, uint8 ExpType, int32 Exp, const FString& Reason, FString& Error);

    /** 覆写事件监听 */
    virtual TArray<TSubclassOf<class UGameEventBase>> GetHandleEventTypes() override;
    virtual void OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) override;
};