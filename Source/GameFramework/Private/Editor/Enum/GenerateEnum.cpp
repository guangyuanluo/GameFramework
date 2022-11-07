// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GenerateEnum.h"

UGenerateEnum::UGenerateEnum(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

}

void UGenerateEnum::Serialize(FArchive& Ar) {
	UEnum::Serialize(Ar);
}

bool UGenerateEnum::SetEnums(TArray<TPair<FName, int64>>& InNames, ECppForm InCppForm, EEnumFlags InFlags, bool bAddMaxKeyIfMissing) {
	CppForm = InCppForm;
	return false;
}

bool UGenerateEnum::SetEnumsEditor(TArray<TPair<FName, int64>>& InNames, ECppForm InCppForm, EEnumFlags InFlags, bool bAddMaxKeyIfMissing) {
	return Super::SetEnums(InNames, InCppForm, InFlags, bAddMaxKeyIfMissing);
}