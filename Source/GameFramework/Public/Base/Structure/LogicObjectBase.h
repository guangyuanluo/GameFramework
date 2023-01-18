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
    * װ��world�����Ķ���
    */
    UFUNCTION(BlueprintCallable)
    void LoadWorldContext(UObject* InWorldContextObject);

    /**
    * ��ȡworld�����Ķ���
    */
    UFUNCTION(BlueprintCallable)
    UObject* GetWorldContextObject() const;

private:
    UPROPERTY()
    UObject* WorldContextObject;
};