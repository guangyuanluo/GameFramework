// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoreGameInstance.generated.h"

class UGameEntityManager;
class UGameSystemManager;

/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API UCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCoreGameInstance(const class FObjectInitializer& ObjectInitializer);

	virtual void Init() override;	
	virtual void Shutdown() override;

	UPROPERTY(BlueprintReadOnly)
	UGameEntityManager* GameEntityManager;

	UPROPERTY(BlueprintReadOnly)
	UGameSystemManager* GameSystemManager;

	/**
	* ��ʼ����Ϸ�߼�
	*/
	UFUNCTION(BlueprintCallable)
	void InitGameLogic();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameSystemManager> SystemManagerClass;

	/**
	* �Ƿ��Զ���ʼ����Ϸ�߼�
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bAutoInitGameLogic = true;

    bool bInitialize = false;
};