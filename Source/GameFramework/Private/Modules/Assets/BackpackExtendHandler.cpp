// Fill out your copyright notice in the Description page of Project Settings.

#include "BackpackExtendHandler.h"

bool UBackpackExtendHandler::AllowItemAdd_Implementation(class UBackpackComponent* BackpackComponent, int ItemID, uint8 BackpackType) {
    return true;
}

void UBackpackExtendHandler::OnItemChange_Implementation(class UBackpackComponent* BackpackComponent, class UCoreItem* NewItem, class UCoreItem* OldItem, uint8 BackpackType, int PackageIndex) {

}

void UBackpackExtendHandler::PreItemEffectAdd_Implementation(const FGameplayEffectSpecHandle& Spec, TSubclassOf<class UGameplayEffect> EffectClass, class UBackpackComponent* BackpackComponent, class UAbilitySystemComponent* AbilitySystemComponent, class UCoreItem* Item) {

}

bool UBackpackExtendHandler::CanMoveItem_Implementation(class UBackpackComponent* BackpackComponent, uint8 SourceBackpackType, int SourceSlotIndex, UCoreItem* SourceItem, uint8 TargetBackpackType, int TargetSlotIndex, UCoreItem* TargetItem) {
    return true;
}