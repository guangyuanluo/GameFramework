// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreItem.h"
#include "Net/UnrealNetwork.h"
#include "BackpackComponent.h"
#include "GameFrameworkUtils.h"
#include "ConfigTableCache.h"
#include "ItemSetting.h"
#include "ItemConfigTableRow.h"
#include "AbilitySystemInterface.h"
#include "BackpackSetting.h"
#include "BackpackExtendHandler.h"
#include "CoreCharacterStateBase.h"
#include "CoreAbilitySystemComponent.h"
#include "SkillSetting.h"
#include "SkillConfigTableRow.h"

void UCoreItem::CheckAndAutoActiveEffects(class UBackpackComponent* BackpackComponent) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
    if (ItemInfo && ItemInfo->AutoActiveSkillWhenInPackage) {
        ActiveEffectsPrivate(BackpackComponent, ItemInfo);
    }
}

void UCoreItem::ActiveEffects(class UBackpackComponent* BackpackComponent) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
    if (ItemInfo) {
        ActiveEffectsPrivate(BackpackComponent, ItemInfo);
    }
}

void UCoreItem::DeactiveEffects(class UBackpackComponent* BackpackComponent) {
    const UItemSetting* ItemSetting = GetDefault<UItemSetting>();
    auto ItemDataTable = ItemSetting->ItemTable.LoadSynchronous();
    auto ItemInfo = (FItemConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ItemDataTable, ItemId);
    if (ItemInfo) {
        if (USkillInfoUtil::IsValid(ItemInfo->ActiveSkill) || ItemInfo->PassiveEffects.Num() > 0) {
            auto CharacterState = Cast<ACoreCharacterStateBase>(BackpackComponent->GetOwner());
            if (CharacterState && CharacterState->GetLocalRole() == ROLE_Authority) {
                auto SkillComponent = CharacterState->SkillComponent;
                if (SkillComponent) {
                    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
                    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
                    if (SkillDataTable) {
                        auto ActiveSkill = (FSkillConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, ItemInfo->ActiveSkill.SkillId);
                        if (ActiveSkill && ActiveSkill->GameplayAbilityClass) {
                            auto FindAbilitySpec = SkillComponent->FindAbilitySpecFromClass(ActiveSkill->GameplayAbilityClass);
                            if (FindAbilitySpec) {
                                SkillComponent->ClearAbility(FindAbilitySpec->Handle);
                            }
                        }
                        if (ItemInfo->PassiveEffects.Num() > 0) {
                            for (int EffectIndex = 0; EffectIndex < ItemInfo->PassiveEffects.Num(); ++EffectIndex) {
                                if (ItemInfo->PassiveEffects[EffectIndex].GameplayEffectClass) {
                                    SkillComponent->RemoveActiveGameplayEffectBySourceEffect(ItemInfo->PassiveEffects[EffectIndex].GameplayEffectClass, SkillComponent, -1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

bool UCoreItem::IsSupportedForNetworking() const {
    return true;
}

void UCoreItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

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

void UCoreItem::ActiveEffectsPrivate(class UBackpackComponent* BackpackComponent, const FItemConfigTableRow* ItemInfo) {
    if (USkillInfoUtil::IsValid(ItemInfo->ActiveSkill) || ItemInfo->PassiveEffects.Num() > 0) {
        auto CharacterState = Cast<ACoreCharacterStateBase>(BackpackComponent->GetOwner());
        if (CharacterState && CharacterState->GetLocalRole() == ROLE_Authority) {
            auto SkillComponent = CharacterState->SkillComponent;
            if (SkillComponent) {
                const UBackpackSetting* BackpackSetting = GetDefault<UBackpackSetting>();
                TSubclassOf<UBackpackExtendHandler> BackpackExtendHandlerClass = UBackpackExtendHandler::StaticClass();
                auto BackpackExtendHandlerClassPath = BackpackSetting->BackpackExtendHandlerClass.ToString();
                if (!BackpackExtendHandlerClassPath.IsEmpty()) {
                    TSubclassOf<UBackpackExtendHandler> LoadClass = StaticLoadClass(UBackpackExtendHandler::StaticClass(), NULL, *BackpackSetting->BackpackExtendHandlerClass.ToString());
                    if (LoadClass) {
                        BackpackExtendHandlerClass = LoadClass;
                    }
                }
                UBackpackExtendHandler* BackpackExtendHandler = Cast<UBackpackExtendHandler>(BackpackExtendHandlerClass->GetDefaultObject());

                const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
                auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
                if (SkillDataTable) {
                    auto ActiveSkill = (FSkillConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, ItemInfo->ActiveSkill.SkillId);
                    if (ActiveSkill && ActiveSkill->GameplayAbilityClass) {
                        SkillComponent->GiveAbility(FGameplayAbilitySpec(ActiveSkill->GameplayAbilityClass, ItemInfo->ActiveSkill.SkillLevel));
                    }
                    if (ItemInfo->PassiveEffects.Num() > 0) {
                        for (int EffectIndex = 0; EffectIndex < ItemInfo->PassiveEffects.Num(); ++EffectIndex) {
                            auto GameplayEffect = ItemInfo->PassiveEffects[EffectIndex].GameplayEffectClass;
                            if (GameplayEffect) {
                                FGameplayEffectContextHandle EffectContext = SkillComponent->MakeEffectContext();
                                FGameplayEffectSpecHandle NewHandle = SkillComponent->MakeOutgoingSpec(GameplayEffect, ItemInfo->PassiveEffects[EffectIndex].EffectLevel, EffectContext);

                                BackpackExtendHandler->PreItemEffectAdd(NewHandle, GameplayEffect, BackpackComponent, SkillComponent, this);

                                if (NewHandle.IsValid()) {
                                    SkillComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}