// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityComponent.h"
#include "GameplayTagContainer.h"
#include "FindEnemyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTargetEnemyUpdate, class UFindEnemyComponent*, FindEnemyComponent, class ACoreCharacter*, OldTarget, class ACoreCharacter*, NewTarget);

class UFindEnemyBase;
class ACoreCharacter;

/**
* 索敌组件
*/
UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UFindEnemyComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFindEnemyComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill", meta = (DisplayName = "索敌类"))
	TSubclassOf<UFindEnemyBase> FindEnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill", meta = (DisplayName = "索敌距离"))
	float FindEnemyDistance = 800.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill", meta = (DisplayName = "丢失目标距离"))
	float DistanceSquareLostEnemy = 4000000.0;


	/**
	 * 是否自动索敌
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
	bool AutoUpdate = false;

	/**
	* 拥有以下Tag时跳过触发自动更新
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill", meta = (DisplayName = "拥有以下Tag时跳过触发自动更新"))
	FGameplayTagContainer IgnoreAutoUpdateTagContainer;

	/**
	* 运行端
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill", DisplayName = "限制的World网络类型", Meta = (ToolTip = "1.Client 2.Server"))
	uint8 LimitNetMode = 1;


	/**
	* 查找或者获取敌人
	*/
	UFUNCTION(BlueprintCallable, Category = "Skill")
	ACoreCharacter* FindOrGetEnemy();

	/**
	* 只是获取敌人
	*/
	UFUNCTION(BlueprintCallable, Category = "Skill")
	ACoreCharacter* JustGetEnemy() const;

	/**
	* 强行设置敌人
	*/
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetEnemy(ACoreCharacter* InEnemy);

	/**
	 * 锁住索敌，不会再切换
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetLock(bool bNewLock);

	/**
	 * 是否锁住索敌
	 */
	UFUNCTION(BlueprintPure, Category = "Skill")
	bool IsLock() const;

	/**
	* 清空敌人
	*/
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ClearEnemy();

	/**
	* 获取索敌实例
	*/
	UFUNCTION(BlueprintPure, Category = "Skill")
	UFindEnemyBase* GetFindEnemyInstance() const;

	/**
	* 索敌变化回调
	*/
	UPROPERTY(BlueprintAssignable)
	FOnTargetEnemyUpdate OnTargetEnemyUpdate;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bLock = false;

	UPROPERTY(Transient)
	ACoreCharacter* Enemy;

	UPROPERTY(Transient)
	UFindEnemyBase* FindEnemyObject;

	/**
	* 向服务器同步索敌
	*/
	UFUNCTION(Server, reliable)
	void ServerSyncEnemy(ACoreCharacter* InEnemy);

	void CheckEnemyLost();
};