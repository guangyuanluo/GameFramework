// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "GMComponent.generated.h"

/**
 * GM×é¼þ
 */
UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEFRAMEWORK_API UGMComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(exec, Category = "Backpack")
	void AddItemInEditor(int32 BackpackType, int32 ItemId, int32 Count, int32 SpecialSlot);

	UFUNCTION(exec, Category = "Backpack")
	void UseItemInEditor(uint8 BackpackType, int SlotIndex, int Count);

	UFUNCTION(exec, Category = "Backpack")
	void AbandonItemInEditor(uint8 BackpackType, int SlotIndex, int Count);

	UFUNCTION(exec, Category = "Backpack")
	void DeductItemInEditor(uint8 BackpackType, int32 ItemId, int Count, int SpecialSlot);

	UFUNCTION(exec, Category = "Backpack")
	void MoveItemInEditor(uint8 BackpackType, int SlotIndex, uint8 NewPackageType, int NewSlotIndex);

	UFUNCTION(exec, Category = "Backpack")
	void SplitItemInEditor(uint8 BackpackType, int SlotIndex, int Count);

	UFUNCTION(exec, Category = "Backpack")
	void PickupItemInEditor(uint8 BackpackType, const FString& DropEntityId, int PickupCount);

    UFUNCTION(exec, Category = "Backpack")
    void SortBackpack(uint8 BackpackType);
};