// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CoreCharacterMovementComponent.generated.h"

UCLASS()
class GAMEFRAMEWORK_API UCoreCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

public:
	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;
};
