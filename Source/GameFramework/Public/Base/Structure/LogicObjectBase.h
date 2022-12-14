#pragma once

#include "CoreMinimal.h"
#include "LogicObjectBase.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API ULogicObjectBase : public UObject
{
    GENERATED_BODY()

public:
    virtual UWorld* GetWorld() const override;

    /**
    * 装载world上下文对象
    */
    UFUNCTION(BlueprintCallable)
    void LoadWorldContext(UObject* InWorldContextObject);

private:
    UPROPERTY()
    UObject* WorldContextObject;
};