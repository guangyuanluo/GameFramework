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
	 * �Ƿ�ia�Ѿ�����
	 */
	UFUNCTION(BlueprintPure)
	bool IsInputActionTrigger(class UInputAction* InputAction) const;

	/**
	 * �Ƿ�һ�����ӳ��ļ�������
	 */
	UFUNCTION(BlueprintPure)
	bool IsPlayerMappableNameInput(const FName& PlayerMappableName) const;

	/**
	* �õ�һ�����ӳ��ļ������ʱ�䣬��λ����
	*/
	UFUNCTION(BlueprintPure)
	bool GetPlayerMappableNameInputTime(const FName& PlayerMappableName, float& InputTime) const;

	/**
	* �õ�һ�����ӳ��ļ��ɿ����ʱ�䣬��λ���룬���û�ɿ������ص���0
	*/
	UFUNCTION(BlueprintPure)
	bool GetPlayerMappableNameInputReleaseTime(const FName& PlayerMappableName, float& InputReleaseTime) const;

	/**
	 * ���߼����ж�ֱ�Ӵ�����ia����¶�����Ա��ṩ��gm����aiʹ��
	 */
	UFUNCTION(BlueprintCallable)
	void AddInputActionTrigger(const class UInputAction* InputAction);

	/**
	 * ���߼����ж��Ƴ���ia����¶�����Ա��ṩ��gm����aiʹ��
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveInputActionTrigger(const class UInputAction* InputAction);

	/**
	 * ���߼����ж�ֱ�Ӵ�����playermappablename�����룬��¶�����Ա��ṩ��gm����aiʹ��
	 */
	UFUNCTION(BlueprintCallable)
	void AddPlayerMappableNameInput(const FName& PlayerMappableName);

	/**
	 * ���߼����ж��Ƴ���playermappablename�����룬��¶�����Ա��ṩ��gm����aiʹ��
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
