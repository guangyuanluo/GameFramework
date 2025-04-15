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
	UPROPERTY(EditDefaultsOnly)
	float ThresholdDistanceBias = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> TestPushObjectTypes;

	UPROPERTY(EditDefaultsOnly)
	float TestPushLength = 5.f;

	UFUNCTION(BlueprintCallable)
	void SetCanBePush(bool bEnable);

	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanBePushed = false;

private:
	TArray<UPrimitiveComponent*> TestPushOverlapComponents;
	TArray<AActor*> TestPushIgnoreActors;

	//防止播动画过程中胶囊体互相嵌入卡死
	virtual void TestAndPushOther(const float DeltaTime);
	virtual void PushAwayOther(const float DeltaTime, class ACharacter* OtherCharacter);
};
