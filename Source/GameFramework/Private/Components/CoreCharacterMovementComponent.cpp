#include "Components/CoreCharacterMovementComponent.h"
#include "Character/CoreCharacter.h"

UCoreCharacterMovementComponent::UCoreCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
}

bool UCoreCharacterMovementComponent::ResolvePenetrationImpl(const FVector & Adjustment, const FHitResult & Hit, const FQuat & NewRotation) {
	auto HitActor = Hit.GetActor();
	if (HitActor && HitActor->IsA(ACoreCharacter::StaticClass())) {
		return false;
	}
	return Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotation);
}