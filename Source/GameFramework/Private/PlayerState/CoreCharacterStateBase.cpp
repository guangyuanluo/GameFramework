// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerState/CoreCharacterStateBase.h"
#include "Modules/Assets/BackpackComponent.h"
#include "Modules/Exp/ExpComponent.h"
#include "Modules/Skill/CoreAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Modules/Quest/QuestComponent.h"
#include "Modules/Player/PlayerComponent.h"
#include "Modules/Assets/WalletComponent.h"
#include "Modules/Scenario/ScenarioComponent.h"

ACoreCharacterStateBase::ACoreCharacterStateBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    //����ͬ��Ƶ�ʣ�0.5sһ���㹻��
    SetNetUpdateFrequency(2);
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
    ScenarioComponent = Cast<UScenarioComponent>(GetComponentByClass((UScenarioComponent::StaticClass())));
}

UAbilitySystemComponent* ACoreCharacterStateBase::GetAbilitySystemComponent() const {
    return SkillComponent;
}

void ACoreCharacterStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACoreCharacterStateBase, SkillComponent);
}