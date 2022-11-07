// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/ECS/GameEntityComponent.h"
#include "Modules/Exp/ExpInfo.h"
#include "Modules/Events/GameEventBase.h"
#include "ExpComponent.generated.h"


UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UExpComponent : public UGameEntityComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExpComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	UPROPERTY(ReplicatedUsing = OnExpChanged, Category = "Exp", EditAnywhere, BlueprintReadOnly)
	TArray<FExpInfo> Exps;					///< 经验

	/**
	* 得到经验类型的等级
	*/
	UFUNCTION(BlueprintCallable)
	int GetExpLevel(uint8 ExpType) const;

	/**
	* 得到经验类型的经验值
	*/
	UFUNCTION(BlueprintCallable)
	int GetExpValue(uint8 ExpType) const;

	/**
	* 增加经验
	*/
	UFUNCTION(BlueprintCallable, Category = "Exp")
	void AddExp(uint8 ExpType, int32 Exp, const FString& Reason);

private:
    UFUNCTION()
    void OnExpChanged();
};

/*************事件定义**************/
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UOnExpRefreshEvent : public UGameEventBase {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
    class ACoreCharacter* Character;
};

UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UAddExpRequesEvent : public UGameEventBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
	FString EntityId;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
	uint8 ExpType;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
	int32 Exp;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventSystem")
	FString Reason;
};