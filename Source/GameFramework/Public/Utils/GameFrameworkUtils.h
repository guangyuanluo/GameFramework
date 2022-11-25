// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "Base/ECS/GameEntity.h"
#include "GameplayEffect.h"
#include "GameFrameworkUtils.generated.h"

class UCoreGameInstance;
class UWaitCondition;
class UCoreCondition;
class AActor;
class ACoreCharacter;

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API UGameFrameworkUtils : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Utils")
	static UWaitCondition* WaitCondition(class ACorePlayerController* PlayerController, const TArray<UCoreCondition*>& Conditions);

	/*
	* @brief 得到半径内最近的角色
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static ACoreCharacter* GetClosestCharacterWithinRadius(ACoreCharacter* Source, float Range, float Radius, ETraceTypeQuery TraceChannel, ETeamAttitude::Type TeamAttitude);

	/*
	* @brief 得到半径内所有的角色
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static TArray<ACoreCharacter*> GetAllCharactersWithinRadius(ACoreCharacter* Source, float Range, float Radius, ETraceTypeQuery TraceChannel, ETeamAttitude::Type TeamAttitude);

	/*
	* @brief 得到半径内最近的actor
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static AActor* GetClosestActorWithinRadius(AActor* Source, float Range, float Radius, ETraceTypeQuery TraceChannel);

	/*
	* @brief 得到半径内所有的actor
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static TArray<AActor*> GetAllActorsWithinRadius(AActor* Source, float Range, float Radius, ETraceTypeQuery TraceChannel);

	/**
	* @brief 合并两个32位整数成64位整数
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static int64 CombineNumber32(int NumberA, int NumberB);

	/** 字符串转int64 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "String To Int64", CompactNodeTitle = "->", BlueprintAutocast), Category = "Utilities|String")
	static int64 Conv_StringToInt64(const FString& InString);

	/** 是否运行在服务端 */
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool IsUE4RunInServer(UObject* WorldContext);

    /**
    * 从技能效果中获得单位
    */
    UFUNCTION(BlueprintCallable, Category = "Utils")
    static class ACoreCharacter* GetCharacterFromGameEffectSpec(const FGameplayEffectSpec& Spec);

    /**
    * 从组件拥有者获取Character
    */
    UFUNCTION(BlueprintPure, Category = "Utils")
    static class ACoreCharacter* GetCharacterFromComponentOwner(class UActorComponent* ActorComponent);

    /**
    * 创建actor
    */
    UFUNCTION(BlueprintCallable, Category = "Utils", meta = (DeterminesOutputType = "ActorClass"))
    static class AActor* SpawnActor(UObject* WorldContext, TSubclassOf<AActor> ActorClass, FVector const& Location, FRotator const& Rotation, ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride);

    /**
    * 获取实体的状态
    */
    UFUNCTION(BlueprintPure, Category = "Utils")
    static class ACoreCharacterStateBase* GetEntityState(TScriptInterface<IGameEntity> Entity);

    /**
    * 获取Actor的状态
    */
    UFUNCTION(BlueprintPure, Category = "Utils")
    static class ACoreCharacterStateBase* GetActorState(class AActor* Actor);

	/**
	* 获取Class的默认对象
	*/
	UFUNCTION(BlueprintPure, Category = "Utils")
	static UObject* GetClassDefaultObject(UClass* InClass);

	/**
	* 获取RoleID
	*/
	UFUNCTION(BlueprintPure, Category = "Utils")
	static FString GetRoleID(class AActor* Actor);

	/**
	* 获取两个角色的队伍属性
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static ETeamAttitude::Type GetTeamAttribute(AActor* ActorA, AActor* ActorB);

	/**
	* 获取EntityID
	*/
	UFUNCTION(BlueprintPure, Category = "Utils")
	static FString GetEntityID(class AActor* Actor);

	/**
	* 获取GameInstance
	*/
	UFUNCTION(BlueprintPure, Category = "Utils")
	static UCoreGameInstance* GetGameInstance(AActor* Actor);

	/**
	* 获取WorldSubsystem
	*/
	UFUNCTION(BlueprintPure, Category = "Utils", meta = (DeterminesOutputType = "SubsystemClass"))
	static class UGameWorldSubsystem* GetGameWorldSubsystem(UObject* WorldContext, TSubclassOf<class UGameWorldSubsystem> SubsystemClass);

	/**
	* 是否默认对象
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool IsTemplateObject(UObject* InObject);

	/**
	* 通过指定class获取任意激活状态下的动画通知状态
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static UAnimNotifyState* GetAnyActiveAnimNotifyStateByClass(USkeletalMeshComponent* MeshComp, TSubclassOf<UAnimNotifyState> AnimNotifyStateClass);

	/**
	* 判断蒙太奇的section是否合法
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool IsMontageValidSection(class UAnimMontage* AnimMontage, FName const& SectionName);

    static TArray<uint8> StringToBinary(const FString& Str);
    static FString BinaryToString(const TArray<uint8>& Data);
    static FString BinaryToString(uint8* Data, int Num);
};