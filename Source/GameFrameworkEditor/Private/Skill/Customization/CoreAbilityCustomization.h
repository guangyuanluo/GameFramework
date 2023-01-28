// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "CoreAbilityCustomization.h"

class FCoreAbilityCustomization : public IDetailCustomization {
public:
	/** Begin IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) override;
	/** End IDetailCustomization interface */
public:

	/** Creates a new instance. */
	static TSharedRef<IDetailCustomization> MakeInstance();
};