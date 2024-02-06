// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "CoreInputCacheComponent.generated.h"

USTRUCT()
struct FPlayerMappableNameInputHistoryInfo {
	GENERATED_BODY()

	FDateTime BeginTime;
	FDateTime EndTime;
	bool bRelease = false;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEFRAMEWORK_API UCoreInputCacheComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyInputStarted, FName, PlayerMappableName);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyInputPressed, FName, PlayerMappableName);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyInputReleased, FName, PlayerMappableName);

	// Sets default values for this component's properties
	UCoreInputCacheComponent();

	virtual void InitializePlayerInput(class UInputComponent* PlayerInputComponent, class UInputMappingContext* MappingContext);

	/**
	 * 是否ia已经触发
	 */
	UFUNCTION(BlueprintPure)
	bool IsInputActionTrigger(class UInputAction* InputAction) const;

	/**
	 * 是否一个玩家映射的键输入了
	 */
	UFUNCTION(BlueprintPure)
	bool IsPlayerMappableNameInput(const FName& PlayerMappableName) const;

	/**
	* 得到一个玩家映射的键输入的时间，单位毫秒
	*/
	UFUNCTION(BlueprintPure)
	bool GetPlayerMappableNameInputTime(const FName& PlayerMappableName, float& InputTime) const;

	/**
	* 得到一个玩家映射的键松开后的时间，单位毫秒，如果没松开，返回的是0
	*/
	UFUNCTION(BlueprintPure)
	bool GetPlayerMappableNameInputReleaseTime(const FName& PlayerMappableName, float& InputReleaseTime) const;

	/**
	 * 让逻辑上判定直接触发了ia，暴露出来以便提供给gm或者ai使用
	 */
	UFUNCTION(BlueprintCallable)
	void AddInputActionTrigger(const class UInputAction* InputAction);

	/**
	 * 让逻辑上判定移除了ia，暴露出来以便提供给gm或者ai使用
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveInputActionTrigger(const class UInputAction* InputAction);

	/**
	 * 让逻辑上判定直接触发了playermappablename的输入，暴露出来以便提供给gm或者ai使用
	 */
	UFUNCTION(BlueprintCallable)
	void AddPlayerMappableNameInput(const FName& PlayerMappableName);

	/**
	 * 让逻辑上判定移除了playermappablename的输入，暴露出来以便提供给gm或者ai使用
	 */
	UFUNCTION(BlueprintCallable)
	void RemovePlayerMappableNameInput(const FName& PlayerMappableName);

	UPROPERTY(BlueprintAssignable)
	FOnKeyInputStarted OnKeyInputStarted;

	UPROPERTY(BlueprintAssignable)
	FOnKeyInputPressed OnKeyInputPressed;

	UPROPERTY(BlueprintAssignable)
	FOnKeyInputReleased OnKeyInputReleased;

private:
	UPROPERTY(Transient)
	TObjectPtr<class UCorePlayerInput> PlayerInput;

	UPROPERTY(Transient)
	TMap<FName, FPlayerMappableNameInputHistoryInfo> PlayerMappableNamesInputHistory;

	UPROPERTY(Transient)
	TSet<const class UInputAction*> TriggerActionInputThisTick;

	void ActionBindingStarted(const FInputActionInstance& Instance);
	void ActionBindingTrigger(const FInputActionInstance& Instance);
	void ActionBindingComplete(const FInputActionInstance& Instance);
};
