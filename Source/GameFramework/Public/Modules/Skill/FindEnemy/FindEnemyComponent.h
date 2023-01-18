// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityComponent.h"
#include "FindEnemyComponent.generated.h"

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
	* 清空敌人
	*/
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ClearEnemy();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(Transient)
	ACoreCharacter* Enemy;

	UPROPERTY(Transient)
	UFindEnemyBase* FindEnemyObject;
};