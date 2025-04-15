#include "Components/CoreCharacterMovementComponent.h"
#include "Character/CoreCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UCoreCharacterMovementComponent::UCoreCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	
}

void UCoreCharacterMovementComponent::SetCanBePush(bool bEnable) {
	bCanBePushed = bEnable;
}

bool UCoreCharacterMovementComponent::ResolvePenetrationImpl(const FVector & Adjustment, const FHitResult & Hit, const FQuat & NewRotation) {
	auto HitActor = Hit.GetActor();
	if (HitActor && HitActor->IsA(ACoreCharacter::StaticClass())) {
		return false;
	}
	return Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotation);
}

void UCoreCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TestAndPushOther(DeltaTime);
}

void UCoreCharacterMovementComponent::TestAndPushOther(const float DeltaTime) {
	if (!bCanBePushed) {
		return;
	}
	if (!CharacterOwner->IsLocallyControlled() && !CharacterOwner->HasAuthority()) {
		return;
	}
	if (!IsMovingOnGround()) {
		return;
	}
	//播动画过程不push
	if (CharacterOwner->IsPlayingRootMotion()) {
		return;
	}
	if (TestPushIgnoreActors.Num() == 0) {
		TestPushIgnoreActors.Add(CharacterOwner);
	}

	const bool HaveFind = UKismetSystemLibrary::CapsuleOverlapComponents(
		this,
		CharacterOwner->GetActorLocation(),
		CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + 30.0f + ThresholdDistanceBias,
		CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		TestPushObjectTypes,
		UCapsuleComponent::StaticClass(),
		TestPushIgnoreActors,
		TestPushOverlapComponents
	);
	if (!HaveFind) {
		return;
	}
	for (const UPrimitiveComponent* OtherComponent : TestPushOverlapComponents)
	{
		ACharacter* OtherCharacter = Cast<ACharacter>(OtherComponent->GetOwner());
		if (OtherCharacter && OtherCharacter != CharacterOwner) {
			PushAwayOther(DeltaTime, OtherCharacter);
		}
	}
	TestPushOverlapComponents.Empty();
}

void UCoreCharacterMovementComponent::PushAwayOther(const float DeltaTime, class ACharacter* OtherCharacter) {
	const float TargetPosBias = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + OtherCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() + 30.f + ThresholdDistanceBias;
	FVector PosBias = CharacterOwner->GetMesh()->GetComponentLocation() - OtherCharacter->GetMesh()->GetComponentLocation();
	if (FMath::Abs(PosBias.Z) > CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
	{
		return;
	}
	PosBias.Z = 0;
	if (!PosBias.IsNearlyZero() && PosBias.SizeSquared() < FMath::Square(TargetPosBias - 0.01f))
	{
		FHitResult OutSweepHit;
		FVector AdjustOffset = PosBias.GetSafeNormal() * FMath::Min(TargetPosBias - PosBias.Size(), TestPushLength);
		UpdatedComponent->AddWorldOffset(AdjustOffset, true, &OutSweepHit);
	}
}