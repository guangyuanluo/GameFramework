// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Data/Team/TeamIdDefines.h"
#include "AbilitySystemInterface.h"
#include "Base/ECS/GameEntity.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "InputActionValue.h"
#include "CoreCharacter.generated.h"

UCLASS()
class GAMEFRAMEWORK_API ACoreCharacter : public ACharacter, public IGenericTeamAgentInterface, public IGameEntity, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACoreCharacter(const class FObjectInitializer& ObjectInitializer);

    /** 队伍id */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TeamIdDefines TeamID;

    /** 模板id */
	UPROPERTY(replicatedUsing = OnRep_TemplateID, BlueprintReadOnly, VisibleAnywhere, Category = "Character")
	int32 TemplateID;

    /** PlayerState Class */
    UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
    TSubclassOf<APlayerState> PlayerStateClass;

    /** 技能模板初始化回调 */
    UFUNCTION(BlueprintNativeEvent, Category = "Character")
    void PostSkillTemplateInit();

    /** 属性变化回调 */
    UFUNCTION(BlueprintNativeEvent, Category = "Character")
    void ReceiveAttributeChanged(FGameplayAttribute Attribute, float OldValue, float NewValue);

    /** 联网同步播放蒙太奇 */
    UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
    void RPC_ReplicatePlayMontageToActorOwingClient(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

    /** 添加角色输入 */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void AddInputContext(const TScriptInterface<class IEnhancedInputSubsystemInterface>& SubsystemInterface);

    /** 移除角色输入 */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void RemoveInputContext(const TScriptInterface<class IEnhancedInputSubsystemInterface>& SubsystemInterface);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

    /** 覆写接口 */
    virtual FString& EntityID_Implementation() override;
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;
    virtual void OnRep_Controller() override;
    virtual void OnRep_PlayerState() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;    

private:
    UPROPERTY(ReplicatedUsing = OnReq_EntityID)
    FString EntityID;

    UPROPERTY()
    bool bSkillInit = false;

    UPROPERTY(Category = "Input", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<class UCoreInputCacheComponent> CoreInputCacheComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputMappingContext* CommonMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* LookDeltaAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    class UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    int32 CommonMappingPriority = 1;

    UFUNCTION()
    void OnReq_EntityID(const FString& OldID);
	
	UFUNCTION()
	void OnRep_TemplateID();

    /**
    * 属性变化回调
    */
    void OnAttributeChange(const struct FOnAttributeChangeData& Data);

    /** 初始模板 */
    void InitTemplate(int InTemplateId);
    /** 初始化技能相关 */
    void InitSkill();
    /** 监听属性变化事件 */
    void ListenAttributeChange();

    void MoveActionBinding(const struct FInputActionValue& ActionValue);
    void LookActionBinding(const struct FInputActionValue& ActionValue);
    void LookDeltaActionBinding(const struct FInputActionValue& ActionValue);
    void Move(const FVector2D& Value);
    void Look(const FVector2D& Value);
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

protected:
    virtual void JumpTrigger(const FInputActionValue& Value);
};