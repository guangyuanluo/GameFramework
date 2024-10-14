// Fill out your copyright notice in the Description page of Project Settings.

#include "BackpackExtendHandler.h"

bool UBackpackExtendHandler::AllowItemAdd_Implementation(class UBackpackComponent* BackpackComponent, int ItemID, EBackpackTypeEnum BackpackType) {
    return true;
}

void UBackpackExtendHandler::OnItemChange_Implementation(class UBackpackComponent* BackpackComponent, class UCoreItem* NewItem, class UCoreItem* OldItem, EBackpackTypeEnum BackpackType, int PackageIndex) {

}

void UBackpackExtendHandler::PreItemEffectAdd_Implementation(const FGameplayEffectSpecHandle& Spec, TSubclassOf<class UGameplayEffect> EffectClass, class UBackpackComponent* BackpackComponent, class UAbilitySystemComponent* AbilitySystemComponent, class UCoreItem* Item) {

}

bool UBackpackExtendHandler::CanMoveItem_Implementation(class UBackpackComponent* BackpackComponent, EBackpackTypeEnum SourceBackpackType, int SourceSlotIndex, UCoreItem* SourceItem, EBackpackTypeEnum TargetBackpackType, int TargetSlotIndex, UCoreItem* TargetItem) {
    return true;
}