// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncTypeWriting.generated.h"

class UScenario;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTypeWritingDelegate, const FString&, NowText);

/**
* 播放剧情的异步函数
*/
UCLASS(meta = (HideThen = true))
class GAMEFRAMEWORK_API UAsyncTypeWriting : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true", WorldContext = "WorldContextObject"))
	static UAsyncTypeWriting* StartTypeWriting(UObject* WorldContextObject, const FString& Text, float PerCharSecond = 0.07f);

public:

	UPROPERTY(BlueprintAssignable)
	FTypeWritingDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
	FTypeWritingDelegate OnTextChange;

	/**
	* 完整要显示的文本
	*/
    UPROPERTY(BlueprintReadOnly)
    FString Text;

	/**
	* 现在需要显示的文本
	*/
	UPROPERTY(BlueprintReadOnly)
	FString NowText;

	/**
	* 显示一个字符需要的时间
	*/
	UPROPERTY(BlueprintReadOnly)
	float PerCharSecond;

    void Start(const FString& InText, float InPerCharSecond);

private:
    struct FTimerHandle TimerHandle;

    UFUNCTION()
	void TimerCallback();

#if WITH_EDITORONLY_DATA
    FDelegateHandle EndPlayDelegateHandle;

    void OnMapChange(uint32 MapChangeFlags);
#endif
};
