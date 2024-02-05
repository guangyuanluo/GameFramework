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

	/**
	 * 是否自动索敌，如果是AI，关闭此选项，直接使用SetEnemy强行设置敌人
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill")
	bool AutoUpdate = true;

	/**
	* 拥有以下Tag时跳过触发自动更新
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill", meta = (DisplayName = "拥有以下Tag时跳过触发自动更新"))
	FGameplayTagContainer IgnoreAutoUpdateTagContainer;

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
	* 更新自动索敌间隔
	*/
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetAutoUpdateInterval(float NewInterval);

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
	FDateTime LastAutoUpdateTime;
	float AutoUpdateInterval = 500.f;

	UPROPERTY(Transient)
	ACoreCharacter* Enemy;

	UPROPERTY(Transient)
	UFindEnemyBase* FindEnemyObject;

	/**
	* 向服务器同步索敌
	*/
	UFUNCTION(Server, reliable)
	void ServerSyncEnemy(ACoreCharacter* InEnemy);
};