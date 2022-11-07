// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/Factory.h"
#include "GenerateEnumFactory.generated.h"

UCLASS(hidecategories=Object, MinimalAPI)
class UGenerateEnumFactory : public UFactory
{
    GENERATED_UCLASS_BODY()

public:
    //~ Begin UFactory Interface
    virtual bool ShouldShowInNewMenu() const override;
	virtual UObject* FactoryCreateNew(UClass* Class,UObject* InParent,FName Name,EObjectFlags Flags,UObject* Context,FFeedbackContext* Warn) override;
	//~ Begin UFactory Interface
};