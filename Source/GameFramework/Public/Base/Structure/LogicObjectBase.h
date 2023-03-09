#pragma once

#include "CoreMinimal.h"
#include "LogicObjectBase.generated.h"

USTRUCT(BlueprintType)
struct GAMEFRAMEWORK_API FLogicObjectLoadWorldScope {
	GENERATED_BODY()

    FLogicObjectLoadWorldScope();
    FLogicObjectLoadWorldScope(class ULogicObjectBase* InLogicObjectBase, class UObject* InWorldContext);
	~FLogicObjectLoadWorldScope();

    void Reload();

	class ULogicObjectBase* LogicObjectBase;

	UPROPERTY()
	class UObject* WorldContext;
};

UCLASS(BlueprintType, Blueprintable)
class GAMEFRAMEWORK_API ULogicObjectBase : public UObject
{
    GENERATED_BODY()

public:
    virtual UWorld* GetWorld() const override;
    
    /**
    * 获取world上下文对象
    */
    UFUNCTION(BlueprintCallable)
    UObject* GetWorldContextObject() const;

private:
    friend struct FLogicObjectLoadWorldScope;

    UPROPERTY()
    UObject* WorldContextObject;

    void LoadWorldContext(UObject* InWorldContextObject);
};

UCLASS()
class GAMEFRAMEWORK_API ULogicObjectUtils : public UBlueprintFunctionLibrary {
public:
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category = "Utils")
    static void LogicObjectLoadWorld(const FLogicObjectLoadWorldScope& Scope, class ULogicObjectBase* InLogicObjectBase, class UObject* InWorldContext);

};