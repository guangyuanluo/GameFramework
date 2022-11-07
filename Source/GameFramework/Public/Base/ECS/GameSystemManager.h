// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/SystemBase.h"
#include "GameSystemManager.generated.h"

class UCoreGameInstance;
class UGameSystemFeature;

USTRUCT()
struct FGameSystemInfo {
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    USystemBase* SystemInstance;

    int32 RefCount = 0;
    bool IsInitialize = false;
};

/*
* @brief 系统管理器
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API UGameSystemManager : public UObject {
	GENERATED_BODY()

public:
	void Initialize(UCoreGameInstance* InGameInstance);
    void Uninitialize();

    /**
    * 蓝图获取system
    */
    UFUNCTION(BlueprintCallable, Category = "Base", meta = (SystemClass = "SystemBase", DisplayName = "GetSystemByClass", DeterminesOutputType = "SystemClass"))
    USystemBase* K2_GetSystemByClass(TSubclassOf<USystemBase> SystemClass) const;

    /**
    * 增加系统特性
    */
    UFUNCTION(BlueprintCallable, Category = "Base")
    void AddGameSystemFeature(TSubclassOf<UGameSystemFeature> SystemFeatureClass);

    /**
    * 移除系统特性
    */
    UFUNCTION(BlueprintCallable, Category = "Base")
    void RemoveGameSystemFeature(TSubclassOf<UGameSystemFeature> SystemFeatureClass);

    /**
    * c++ 通过模板获取system
    */
	template<class SystemType>
	SystemType* GetSystemByClass() const {
        return Cast<SystemType>(K2_GetSystemByClass(SystemType::StaticClass()));
	}

protected:
    UCoreGameInstance* GameInstance;

    UPROPERTY()
    TMap<UClass*, FGameSystemInfo> SystemMap;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UGameSystemFeature> DefaultFeature;
};