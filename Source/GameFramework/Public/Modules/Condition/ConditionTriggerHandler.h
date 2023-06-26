// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ConditionTriggerHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllProgressesSatisfyDynamicDelegate);

/** Unique handle that can be used to distinguish timers that have identical delegates. */
USTRUCT(BlueprintType)
struct FConditionTriggerHandler {
	GENERATED_BODY()

	FConditionTriggerHandler()
		: Handle(0) {
	}

	FConditionTriggerHandler(const FConditionTriggerHandler& Other) {
		Handle = Other.Handle;
		OnAllProgressesSatisfy = Other.OnAllProgressesSatisfy;
	}
	FConditionTriggerHandler& operator=(const FConditionTriggerHandler& Other) {
		Handle = Other.Handle;
		OnAllProgressesSatisfy = Other.OnAllProgressesSatisfy;
		return *this;
	}

	/**
	* 所有进度满足时的回调
	*/
	UPROPERTY(BlueprintAssignable)
	FOnAllProgressesSatisfyDynamicDelegate OnAllProgressesSatisfy;

	/** True if this handle was ever initialized by the timer manager */
	bool IsValid() const {
		return Handle != 0;
	}

	/** Explicitly clear handle */
	void Invalidate() {
		Handle = 0;
	}

	bool operator==(const FConditionTriggerHandler& Other) const {
		return Handle == Other.Handle;
	}

	bool operator!=(const FConditionTriggerHandler& Other) const {
		return Handle != Other.Handle;
	}

	FString ToString() const {
		return FString::Printf(TEXT("%llu"), Handle);
	}

private:
	friend class UConditionTriggerSystem;

	static constexpr uint32 SerialNumberBits = 40;
	static constexpr uint64 MaxSerialNumber = (uint64)1 << SerialNumberBits;

	void SetInHandle(int32 InHandle) {		
		Handle = InHandle;
	}

	UPROPERTY(Transient)
	uint64 Handle;

	friend uint32 GetTypeHash(const FConditionTriggerHandler& InHandle) {
		return GetTypeHash(InHandle.Handle);
	}
};