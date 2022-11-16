// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "SystemBase.generated.h"

class UCoreGameInstance;

//需要标记枚举，并设置蓝图类型  
UENUM(BlueprintType)
enum class ESystemType :uint8 //设置uint8类型  
{
    E_Both UMETA(DisplayName = "双端"),
    E_ServerOnly UMETA(DisplayName = "仅服务端"),
    E_ClientOnly UMETA(DisplayName = "仅客户端"),
};

/*
* @brief 系统基类
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API USystemBase : public UObject, public FTickableGameObject {
	GENERATED_BODY()

public:
	virtual void Initialize(UCoreGameInstance* InGameInstance);
    virtual void Uninitialize();

    UFUNCTION(BlueprintNativeEvent, Category = "System")
    void PostInitialize();

    UFUNCTION(BlueprintNativeEvent, Category = "System")
    void PreUninitialize();

    UFUNCTION(BlueprintNativeEvent, Category = "System")
    void OnTick(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "System")
    ESystemType GetSystemType() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "系统类型"))
    ESystemType SystemType = ESystemType::E_Both;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "System")
    UCoreGameInstance* GameInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "是否需要tick"))
    bool CanTick = false;

    // FTickableGameObject begin
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual bool IsTickableWhenPaused() const override { return false; }
    virtual bool IsTickableInEditor() const override { return false; }
    virtual ETickableTickType GetTickableTickType() const override;
    virtual bool IsTickable() const override;
    // FTickableGameObject end

    virtual UWorld* GetWorld() const override;
};