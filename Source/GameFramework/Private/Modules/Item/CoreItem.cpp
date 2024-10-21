// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreItem.h"
#include "Net/UnrealNetwork.h"
#include "BackpackComponent.h"
#include "CoreCharacterStateBase.h"
#include "ConfigTableCache.h"
#include "ItemSetting.h"
#include "ItemConfigTableRow.h"
#include "CoreAbilitySystemComponent.h"

void UCoreItem::ActiveEffects(class UBackpackComponent* BackpackComponent, const FItemEffectPreAddDelegate& InEffectPreAddCallback) {
    auto Callback = FEffectPreAddDelegate::CreateLambda([this, InEffectPreAddCallback](class UCoreAbilitySystemComponent* SkillComponent, const FGameplayEffectSpecHandle& Spec, TSubclassOf<class UGameplayEffect> EffectClass) {
        InEffectPreAddCallback.ExecuteIfBound(SkillComponent, Spec, EffectClass, this);
    });
    ActiveActorEffectsPrivate(BackpackComponent, Callback);
    ActiveItemTableEffectsPrivate(BackpackComponent, Callback);
}

void UCoreItem::ActiveActorEffects(class UBackpackComponent* BackpackComponent, const FItemEffectPreAddDelegate& InEffectPreAddCallback) {
    auto Callback = FEffectPreAddDelegate::CreateLambda([this, InEffectPreAddCallback](class UCoreAbilitySystemComponent* SkillComponent, const FGameplayEffectSpecHandle& Spec, TSubclassOf<class UGameplayEffect> EffectClass) {
        InEffectPreAddCallback.ExecuteIfBound(SkillComponent, Spec, EffectClass, this);
    });
    ActiveActorEffectsPrivate(BackpackComponent, Callback);
}

void UCoreItem::ActiveItemTableEffects(class UBackpackComponent* BackpackComponent, const FItemEffectPreAddDelegate& InEffectPreAddCallback) {
    auto Callback = FEffectPreAddDelegate::CreateLambda([this, InEffectPreAddCallback](class UCoreAbilitySystemComponent* SkillComponent, const FGameplayEffectSpecHandle& Spec, TSubclassOf<class UGameplayEffect> EffectClass) {
        InEffectPreAddCallback.ExecuteIfBound(SkillComponent, Spec, EffectClass, this);
    });
    ActiveItemTableEffectsPrivate(BackpackComponent, Callback);
}

void UCoreItem::DeactiveEffects(class UBackpackComponent* BackpackComponent) {
    DeactiveActorEffects(BackpackComponent);
    DeactiveItemTableEffects(BackpackComponent);
}

void UCoreItem::DeactiveActorEffects(class UBackpackComponent* BackpackComponent) {
    if (Effects.Num()) {
        auto CharacterState = Cast<ACoreCharacterStateBase>(BackpackComponent->GetOwner());
        if (CharacterState && CharacterState->SkillComponent) {
            for (auto& EffectInfo : Effects) {
                CharacterState->SkillComponent->RemoveEffect(EffectInfo);
            }
        }
    }
}

void UCoreItem::DeactiveItemTableEffects(class UBackpackComponent* BackpackComponent) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemRow = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
    if (ItemRow->SkillGroupIDContainer.SkillGroupID != 0) {
        auto CharacterState = Cast<ACoreCharacterStateBase>(BackpackComponent->GetOwner());
        if (CharacterState && CharacterState->SkillComponent) {
            CharacterState->SkillComponent->RemoveSkillGroup(ItemRow->SkillGroupIDContainer.SkillGroupID);
        }
    }
}

bool UCoreItem::IsSupportedForNetworking() const {
    return true;
}

void UCoreItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCoreItem, InstanceID);
    DOREPLIFETIME(UCoreItem, ItemId);
    DOREPLIFETIME(UCoreItem, ItemNum);
}

void UCoreItem::OnRep_ItemNum() {
    auto Outer = Cast<AActor>(GetOuter());
    if (Outer) {
        auto CharacterState = Cast<ACoreCharacterStateBase>(Outer);
        if (CharacterState && CharacterState->BackpackComponent) {
            CharacterState->BackpackComponent->NotifyBackpackChanged();
        }
    }
}

void UCoreItem::ActiveActorEffectsPrivate(class UBackpackComponent* BackpackComponent, const FEffectPreAddDelegate& Callback) {
    if (Effects.Num()) {
        auto CharacterState = Cast<ACoreCharacterStateBase>(BackpackComponent->GetOwner());
        if (CharacterState && CharacterState->SkillComponent) {
            for (auto& EffectInfo : Effects) {
                CharacterState->SkillComponent->AddEffect(EffectInfo, Callback);
            }
        }
    }
}

void UCoreItem::ActiveItemTableEffectsPrivate(class UBackpackComponent* BackpackComponent, const FEffectPreAddDelegate& Callback) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemRow = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
    if (ItemRow->SkillGroupIDContainer.SkillGroupID != 0) {
        auto CharacterState = Cast<ACoreCharacterStateBase>(BackpackComponent->GetOwner());
        if (CharacterState && CharacterState->SkillComponent) {
            CharacterState->SkillComponent->AddSkillGroup(ItemRow->SkillGroupIDContainer.SkillGroupID, Callback);
        }
    }
}