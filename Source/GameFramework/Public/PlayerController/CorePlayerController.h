// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CorePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterTemplateUpdatedSignature, class ACorePlayerController*, PlayerController);
/**
 * 
 */
UCLASS()
class GAMEFRAMEWORK_API ACorePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACorePlayerController(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterController")
	float WalkSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterController")
	float MouseSensibility = 1.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterController")
	bool EnableInteractive;				///< 是否允许交互

    /** GM组件 */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterController")
    class UGMComponent* GMComponent;

	/**
	* 角色模板变化通知
	*/
	UPROPERTY(BlueprintAssignable, Category = Character)
	FCharacterTemplateUpdatedSignature OnCharacterTemplateUpdated;

	/**
	* 初始化role id
	*/
	UFUNCTION(BlueprintCallable)
	void InitRoleId(const FString& RoleID);

    /**
    * 修改角色模板id
    */
    UFUNCTION(BlueprintCallable)
    void ChangeCharacterTemplateId(int TemplateId);

	/**
	* @brief 加载服务器地图
	*/
	UFUNCTION(BlueprintCallable)
	void EnterGame(const FString& InName);

	UFUNCTION(Server, reliable)
	void SendEventToServerReliable(const FString& EventClass, const FString& SerializeEvent);
	void SendEventToServerReliable_Implementation(const FString& EventClass, const FString& SerializeEvent);

	UFUNCTION(Server, unreliable)
	void SendEventToServerUnreliable(const FString& EventClass, const FString& SerializeEvent);
	void SendEventToServerUnreliable_Implementation(const FString& EventClass, const FString& SerializeEvent);

	UFUNCTION(Client, reliable)
	void SendEventToClient(const FString& EventClass, const FString& SerializeEvent);
	void SendEventToClient_Implementation(const FString& EventClass, const FString& SerializeEvent);

    /**
    * 控制角色时客户端回调
    */
    UFUNCTION(BlueprintImplementableEvent)
    void K2_OnClientPossessed();

	UFUNCTION(BlueprintCallable)
	void JumpExecute();
	UFUNCTION(BlueprintCallable)
	void StopJumpingExecute();

public:
	/** 覆写接口 */
    virtual void OnRep_Pawn() override;

private:
	FVector2D LastTouchLocation;

protected:
    virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;
	virtual void OnPossess(APawn* InPawn) override;

	void MoveVertical(float Value);
	void MoveHorizontal(float Value);
	virtual void Yaw(float Value);
	virtual void Pitch(float Value);
	virtual void Jump();
	virtual void StopJumping();
	virtual void OnTouchBegin(ETouchIndex::Type FingerIndex, FVector Location);
	virtual void OnTouchEnd(ETouchIndex::Type FingerIndex, FVector Location);
	virtual void OnFingerMove(ETouchIndex::Type FingerIndex, FVector Location);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
};