#pragma once

#include "CoreMinimal.h"
#include "LogicObjectBase.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API ULogicObjectBase : public UObject
{
    GENERATED_BODY()

public:
    virtual UWorld* GetWorld() const override;
};