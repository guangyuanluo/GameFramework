// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreCharacter.h"
#include "Engine/World.h"
#include "CoreGameInstance.h"
#include "Net/UnrealNetwork.h"

#include "Runtime/AIModule/Classes/Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "Runtime/Engine/Classes/Animation/AnimSequence.h"
#include "Engine/ActorChannel.h"
#include "GameEntityManager.h"
#include "UE4LogImpl.h"
#include "ExpComponent.h"
#include "AbilitySystemGlobals.h"
#include "ChangeAttributeEvent.h"
#include "EventSystem.h"
#include "GameSystemManager.h"
#include "CoreCharacterStateBase.h"
#include "CoreAbilitySystemComponent.h"
#include "ConfigTableCache.h"
#include "ExpSetting.h"
#include "UnitSetting.h"
#include "UnitInfoConfigTableRow.h"
#include "Kismet/GameplayStatics.h"

ACoreCharacter::ACoreCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    
}

void ACoreCharacter::BeginPlay() {
    Super::BeginPlay();

	if (TemplateID != 0) {
        InitTemplate(TemplateID);
	}

    auto CharacterState = Cast<ACoreCharacterStateBase>(GetPlayerState());
    if (CharacterState) {
        auto SkillComponent = CharacterState->SkillComponent;
        if (GetLocalRole() == ENetRole::ROLE_Authority) {
            if (!SkillComponent->AbilityActorInfo.IsValid()) {
                SkillComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(UAbilitySystemGlobals::Get().AllocAbilityActorInfo());
            }
            SkillComponent->InitAbilityActorInfo(CharacterState, this);
        }
    }

    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    GameInstance->GameEntityManager->OnEntityAdd(this);
}

void ACoreCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

    auto World = GetWorld();
    if (World) {
        auto GameInstance = World->GetGameInstance<UCoreGameInstance>();
        if (GameInstance && GameInstance->GameEntityManager) {
            GameInstance->GameEntityManager->OnEntityRemove(this);
        }
    }
}

void ACoreCharacter::OnReq_EntityID(const FString& OldID) {
    auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
    GameInstance->GameEntityManager->OnEntityIDChanged(this, OldID);
}

void ACoreCharacter::OnRep_TemplateID() {
    
}

void ACoreCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(ACoreCharacter, EntityID, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(ACoreCharacter, TemplateID);
}

FString& ACoreCharacter::EntityID_Implementation() {
    return EntityID;
}

void ACoreCharacter::PossessedBy(AController* NewController) {
    Super::PossessedBy(NewController);

    // Initialize our abilities
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetPlayerState());
    if (CharacterState && CharacterState->SkillComponent) {
        if (CharacterState->SkillComponent) {
            if (GetLocalRole() == ENetRole::ROLE_Authority) {
                CharacterState->SkillComponent->RefreshAbilityActorInfo();
            }
        }
    }
}

void ACoreCharacter::UnPossessed() {
    Super::UnPossessed();
}

void ACoreCharacter::OnRep_Controller() {
    Super::OnRep_Controller();
}

void ACoreCharacter::OnRep_PlayerState() {
    Super::OnRep_PlayerState();

    // Our controller changed, must update ActorInfo on AbilitySystemComponent
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetPlayerState());
    if (CharacterState && CharacterState->SkillComponent) {
        if (GetLocalRole() != ENetRole::ROLE_Authority) {
            if (!CharacterState->SkillComponent->AbilityActorInfo.IsValid()) {
                CharacterState->SkillComponent->AbilityActorInfo = TSharedPtr<FGameplayAbilityActorInfo>(UAbilitySystemGlobals::Get().AllocAbilityActorInfo());
            }
            CharacterState->SkillComponent->InitAbilityActorInfo(CharacterState, this);

            InitSkill();
        }
    }
}

void ACoreCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID) {
    TeamID = (TeamIdDefines)InTeamID.GetId();
}

FGenericTeamId ACoreCharacter::GetGenericTeamId() const {
	return (uint8)TeamID;
}

void ACoreCharacter::OnAttributeChange(const FOnAttributeChangeData& Data) {
    if (Data.OldValue != Data.NewValue) {
        //属性变化事件
        UChangeAttributeEvent* ChangeAttributeEvent = NewObject<UChangeAttributeEvent>();
        ChangeAttributeEvent->Character = this;
        ChangeAttributeEvent->Attribute = Data.Attribute;
        ChangeAttributeEvent->OldValue = Data.OldValue;
        ChangeAttributeEvent->NewValue = Data.NewValue;

        //蓝图回调
        ReceiveAttributeChanged(Data.Attribute, Data.OldValue, Data.NewValue);

        //事件广播
        auto GameInstance = GetWorld()->GetGameInstance<UCoreGameInstance>();
        GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(ChangeAttributeEvent);
    }
}

void ACoreCharacter::InitTemplate(int InTemplateID) {
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable == nullptr) {
        UE_LOG(GameCore, Error, TEXT("初始化单位模板失败，没有配置单位表"));
        return;
    }
    auto FindUnitInfo = (FUnitInfoConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(UnitDataTable, InTemplateID);
    if (FindUnitInfo == nullptr) {
        UE_LOG(GameCore, Error, TEXT("初始化单位模板失败，没找到单位模板id:%d"), InTemplateID);
        return;
    }
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetPlayerState());
    if (!CharacterState) {
        if (GetNetMode() != NM_Client) {
            FActorSpawnParameters SpawnInfo;
            SpawnInfo.Owner = this;
            SpawnInfo.Instigator = GetInstigator();
            SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            SpawnInfo.ObjectFlags |= RF_Transient;	// We never want player states to save into a map

            TSubclassOf<APlayerState> PlayerStateClassToSpawn = PlayerStateClass;
            if (PlayerStateClassToSpawn.Get() == nullptr) {
                PlayerStateClassToSpawn = ACoreCharacterStateBase::StaticClass();
            }

            CharacterState = Cast<ACoreCharacterStateBase>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, PlayerStateClassToSpawn, FTransform::Identity, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
            SetPlayerState(CharacterState);
            UGameplayStatics::FinishSpawningActor(CharacterState, FTransform::Identity);
        }
    }
    if (!CharacterState) {
        return;
    }
    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();
    if (ExpTypeDataTable) {
        if (UConfigTableCache::GetDataTableRawDataFromId(ExpTypeDataTable, FindUnitInfo->GrowExpTypeId)) {
            if (GetLocalRole() == ENetRole::ROLE_Authority) {
                FExpInfo ExpInfo;
                ExpInfo.ExpType = FindUnitInfo->GrowExpTypeId;
                ExpInfo.Level = FindUnitInfo->Level;
                CharacterState->ExpComponent->Exps.Add(ExpInfo);
            }
        }
    }

    InitSkill();
}

void ACoreCharacter::InitSkill() {
    if (bSkillInit) return;
    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable) {
        auto FindUnitInfo = (FUnitInfoConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(UnitDataTable, TemplateID);
        if (FindUnitInfo) {
            if (FindUnitInfo->SkillGroupID != -1
                || !FindUnitInfo->AttributeTable.GetLongPackageName().IsEmpty()) {
                auto CharacterState = Cast<ACoreCharacterStateBase>(GetPlayerState());
                CharacterState->SkillComponent->InitSkillFromTemplate(TemplateID);
                ListenAttributeChange();
                PostSkillTemplateInit();
                CharacterState->SkillComponent->OnSkillTemplatePostInit.Broadcast(CharacterState->SkillComponent);
                bSkillInit = true;
            }
        }
    }
}

void ACoreCharacter::ListenAttributeChange() {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetPlayerState());
    /** 监听所有属性变化，替换成事件 */
    TArray<FGameplayAttribute> AllGameplayAttributes;

    CharacterState->SkillComponent->GetAllAttributes(AllGameplayAttributes);
    for (int AttributeIndex = 0; AttributeIndex < AllGameplayAttributes.Num(); ++AttributeIndex) {
        CharacterState->SkillComponent->GetGameplayAttributeValueChangeDelegate(AllGameplayAttributes[AttributeIndex]).AddUObject(this, &ACoreCharacter::OnAttributeChange);
    }
}

UAbilitySystemComponent* ACoreCharacter::GetAbilitySystemComponent() const {
    auto CharacterState = Cast<ACoreCharacterStateBase>(GetPlayerState());
    if (CharacterState) {
        return CharacterState->SkillComponent;
    }
    else {
        return nullptr;
    }
}