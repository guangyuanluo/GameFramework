// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
class GAMEFRAMEWORK_API USystemBase : public UObject {
	GENERATED_BODY()

public:
	virtual void Initialize(UCoreGameInstance* InGameInstance);
    virtual void Uninitialize();

    UFUNCTION(BlueprintImplementableEvent, Category = "System")
    void PostInitialize();

    UFUNCTION(BlueprintImplementableEvent, Category = "System")
    void PreUninitialize();

    UFUNCTION(BlueprintPure, Category = "System")
    ESystemType GetSystemType() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", DisplayName = "系统类型"))
    ESystemType SystemType = ESystemType::E_Both;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "System")
    UCoreGameInstance* GameInstance;
};