// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 *	User defined enumerations:
 *	- EnumType is always UEnum::ECppForm::Namespaced (to comfortable handle names collisions)
 *	- always have the last '_MAX' enumerator, that cannot be changed by user
 *	- Full enumerator name has form: '<enumeration path>::<short, user defined enumerator name>'
 */

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "GenerateEnum.generated.h"

 /**
  *	An Enumeration is a list of named values.
  */
UCLASS()
class GAMEFRAMEWORK_API UGenerateEnum : public UUserDefinedEnum {
	GENERATED_UCLASS_BODY()

public:
	//~ Begin UObject Interface.
	virtual void Serialize(FArchive& Ar) override;
	virtual bool SetEnums(TArray<TPair<FName, int64>>& InNames, ECppForm InCppForm, EEnumFlags InFlags = EEnumFlags::None, bool bAddMaxKeyIfMissing = true) override;
	bool SetEnumsEditor(TArray<TPair<FName, int64>>& InNames, ECppForm InCppForm, EEnumFlags InFlags = EEnumFlags::None, bool bAddMaxKeyIfMissing = true);
	//~ End UObject Interface.
};