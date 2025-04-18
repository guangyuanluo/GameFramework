// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "Base/ECS/GameEntity.h"
#include "GameplayEffect.h"
#include "Data/Team/TeamIdDefines.h"
#include "GameFrameworkUtils.generated.h"

class UCoreGameInstance;
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

	/*
	* @brief 得到半径内最近的actor
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils", meta = (DeterminesOutputType = "LimitActorClass"))
	static AActor* GetClosestActorWithinRadius(AActor* Source, const TArray<AActor*>& IgnoreActors, const FVector& OffsetFromActor, float TraceLength, float Radius, FName ProfileName, ETeamAttitude::Type TeamAttitude, TSubclassOf<AActor> LimitActorClass, bool DrawDebug = true);

	/*
	* @brief 得到半径内所有的actor
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils", meta = (DeterminesOutputType = "LimitActorClass"))
	static TArray<AActor*> GetAllActorsWithinRadius(AActor* Source, const TArray<AActor*>& IgnoreActors, const FVector& OffsetFromActor, float TraceLength, float Radius, FName ProfileName, ETeamAttitude::Type TeamAttitude, TSubclassOf<AActor> LimitActorClass, bool SortByDistance = false, bool DrawDebug = true);

	/**
	* @brief 合并两个32位整数成64位整数
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static int64 CombineNumber32(int NumberA, int NumberB);

	/** 是否运行在服务端 */
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool IsUE4RunInServer(UObject* WorldContext);

	/** 是否运行在editor */
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool IsUE4RunInEditor();

    /**
    * 从组件拥有者获取Character
    */
    UFUNCTION(BlueprintPure, Category = "Utils")
    static class ACoreCharacter* GetCharacterFromComponentOwner(class UActorComponent* ActorComponent);

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
	* 获取某个角色与队伍的属性
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static ETeamAttitude::Type GetActorTeamAttributeWithTeam(AActor* ActorA, TeamIdDefines Team);

	/**
	* 获取EntityID
	*/
	UFUNCTION(BlueprintPure, Category = "Utils")
	static FString GetEntityID(class AActor* Actor);

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
	* 获取距离指定动画通知还有多少时间
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool GetDiffTimeToAnyAnimNotifyStateByClass(USkeletalMeshComponent* MeshComp, UAnimMontage* AnimMontage, TSubclassOf<UAnimNotifyState> AnimNotifyStateClass, float& DiffTime);

	/**
	* 判断蒙太奇的section是否合法
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static bool IsMontageValidSection(class UAnimMontage* AnimMontage, FName const& SectionName);

	/**
	* 获取蒙太奇下个section
	*/
	UFUNCTION(BlueprintCallable, Category = "Utils")
	static FName GetMontageNextSection(class UAnimInstance* AnimInstance);
};