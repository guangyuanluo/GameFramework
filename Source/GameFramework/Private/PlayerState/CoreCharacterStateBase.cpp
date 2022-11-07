// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreCharacterStateBase.h"
#include "BackpackComponent.h"
#include "ExpComponent.h"
#include "CoreAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "QuestComponent.h"
#include "PlayerComponent.h"
#include "WalletComponent.h"

ACoreCharacterStateBase::ACoreCharacterStateBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    //降低同步频率，0.5s一次足够了
    NetUpdateFrequency = 2;
}

void ACoreCharacterStateBase::OnConstruction(const FTransform& Transform) {
    Super::OnConstruction(Transform);

    BackpackComponent = Cast<UBackpackComponent>(GetComponentByClass((UBackpackComponent::StaticClass())));
    ExpComponent = Cast<UExpComponent>(GetComponentByClass((UExpComponent::StaticClass())));
    SkillComponent = Cast<UCoreAbilitySystemComponent>(GetComponentByClass((UCoreAbilitySystemComponent::StaticClass())));
    if (SkillComponent) {
        SkillComponent->SetIsReplicated(true);
    }
    QuestComponent = Cast<UQuestComponent>(GetComponentByClass((UQuestComponent::StaticClass())));
    PlayerComponent = Cast<UPlayerComponent>(GetComponentByClass((UPlayerComponent::StaticClass())));
    WalletComponent = Cast<UWalletComponent>(GetComponentByClass((UWalletComponent::StaticClass())));
}

UAbilitySystemComponent* ACoreCharacterStateBase::GetAbilitySystemComponent() const {
    return SkillComponent;
}

void ACoreCharacterStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    if (SkillComponent) {
        DOREPLIFETIME(ACoreCharacterStateBase, SkillComponent);
    }
}