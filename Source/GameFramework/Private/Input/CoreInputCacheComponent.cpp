#include "Input/CoreInputCacheComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedActionKeyMapping.h"
#include "GameFramework/PlayerInput.h"
#include "Input/CorePlayerInput.h"
#include "InputMappingContext.h"

// Sets default values for this component's properties
UCoreInputCacheComponent::UCoreInputCacheComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCoreInputCacheComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent, class UInputMappingContext* MappingContext)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		auto InputPawn = Cast<APawn>(PlayerInputComponent->GetOwner());
		if (!InputPawn) {
			return;
		}
		auto InputPawnControl = Cast<APlayerController>(InputPawn->GetController());
		if (!InputPawnControl) {
			return;
		}
		if (!InputPawnControl->PlayerInput) {
			return;
		}
		PlayerInput = Cast<UCorePlayerInput>(InputPawnControl->PlayerInput);
		if (!ensure(PlayerInput)) {
			return;
		}

		TSet<UInputAction*> InputActionSet;
		const auto& ActionEventBindings = EnhancedInputComponent->GetActionEventBindings();
		for (const auto& ActionEventBinding : ActionEventBindings) {
			auto Action = const_cast<UInputAction*>(ActionEventBinding->GetAction());
			if (Action)
			{
				InputActionSet.Add(Action);
			}
		}
		for (const auto& Mapping : MappingContext->GetMappings()) {			
			if (Mapping.Action)
			{
				InputActionSet.Add(const_cast<UInputAction*>(Mapping.Action.Get()));
			}
		}

		for (auto Iter = InputActionSet.CreateIterator(); Iter; ++Iter)
		{
			EnhancedInputComponent->BindAction(*Iter, ETriggerEvent::Started, this, &ThisClass::ActionBindingStarted);
			EnhancedInputComponent->BindAction(*Iter, ETriggerEvent::Triggered, this, &ThisClass::ActionBindingTrigger);
			EnhancedInputComponent->BindAction(*Iter, ETriggerEvent::Completed, this, &ThisClass::ActionBindingComplete);
		}
	}
}

bool UCoreInputCacheComponent::IsInputActionTrigger(class UInputAction* InputAction) const
{
	return InputAction != nullptr && TriggerActionInputThisTick.Contains(InputAction);
}

bool UCoreInputCacheComponent::IsPlayerMappableNameInput(const FName& PlayerMappableName) const
{
	auto FindHistoryPtr = PlayerMappableNamesInputHistory.Find(PlayerMappableName);
	if (FindHistoryPtr)
	{
		return !FindHistoryPtr->bRelease;
	}
	return false;
}

bool UCoreInputCacheComponent::GetPlayerMappableNameInputTime(const FName& PlayerMappableName, float& InputTime) const
{
	auto FindHistoryPtr = PlayerMappableNamesInputHistory.Find(PlayerMappableName);
	if (FindHistoryPtr && !FindHistoryPtr->bRelease)
	{
		InputTime = (FDateTime::Now() - FindHistoryPtr->BeginTime).GetTotalMilliseconds();
		return true;
	}
	return false;
}

bool UCoreInputCacheComponent::GetPlayerMappableNameInputReleaseTime(const FName& PlayerMappableName, float& InputReleaseTime) const
{
	auto FindHistoryPtr = PlayerMappableNamesInputHistory.Find(PlayerMappableName);
	if (FindHistoryPtr)
	{
		if (FindHistoryPtr->bRelease)
		{
			InputReleaseTime = (FDateTime::Now() - FindHistoryPtr->EndTime).GetTotalMilliseconds();
		}
		else
		{
			InputReleaseTime = 0.f;
		}
		return true;
	}
	return false;
}

void UCoreInputCacheComponent::AddInputActionTrigger(const class UInputAction* InputAction)
{
	TriggerActionInputThisTick.Add(InputAction);
}

void UCoreInputCacheComponent::RemoveInputActionTrigger(const class UInputAction* InputAction)
{
	TriggerActionInputThisTick.Remove(InputAction);
}

void UCoreInputCacheComponent::AddPlayerMappableNameInput(const FName& PlayerMappableName)
{
	auto& HistoryInfo = PlayerMappableNamesInputHistory.FindOrAdd(PlayerMappableName);
	HistoryInfo.BeginTime = FDateTime::Now();
	HistoryInfo.bRelease = false;
	OnKeyInputPressed.Broadcast(PlayerMappableName);
}

void UCoreInputCacheComponent::RemovePlayerMappableNameInput(const FName& PlayerMappableName)
{
	auto HistoryInfoPtr = PlayerMappableNamesInputHistory.Find(PlayerMappableName);
	if (HistoryInfoPtr && !HistoryInfoPtr->bRelease)
	{
		HistoryInfoPtr->EndTime = FDateTime::Now();
		HistoryInfoPtr->bRelease = true;
		OnKeyInputReleased.Broadcast(PlayerMappableName);
	}
}

void UCoreInputCacheComponent::ActionBindingStarted(const FInputActionInstance& Instance) {
	auto TriggerAction = Instance.GetSourceAction();
	const auto& EnhancedActionMappings = PlayerInput->GetEnhancedActionMappingsConst();
	for (const auto& EnhancedActionMapping : EnhancedActionMappings)
	{
		if (EnhancedActionMapping.Action == TriggerAction)
		{
			FName MappableName = EnhancedActionMapping.GetMappingName();
			if (!MappableName.IsNone())
			{
				OnKeyInputStarted.Broadcast(MappableName);
			}
		}
	}
}

void UCoreInputCacheComponent::ActionBindingTrigger(const FInputActionInstance& Instance)
{
	auto TriggerAction = Instance.GetSourceAction();
	const auto& EnhancedActionMappings = PlayerInput->GetEnhancedActionMappingsConst();
	for (const auto& EnhancedActionMapping : EnhancedActionMappings)
	{
		if (EnhancedActionMapping.Action == TriggerAction)
		{
			FName MappableName = EnhancedActionMapping.GetMappingName();
			if (!MappableName.IsNone())
			{
				if (PlayerInput->IsPressed(EnhancedActionMapping.Key))
				{
					AddPlayerMappableNameInput(MappableName);
				}
				else {
					RemovePlayerMappableNameInput(MappableName);
				}
			}
		}
	}
	AddInputActionTrigger(TriggerAction);
}

void UCoreInputCacheComponent::ActionBindingComplete(const FInputActionInstance& Instance)
{
	auto TriggerAction = Instance.GetSourceAction();
	const auto& EnhancedActionMappings = PlayerInput->GetEnhancedActionMappingsConst();
	for (const auto& EnhancedActionMapping : EnhancedActionMappings)
	{
		if (EnhancedActionMapping.Action == TriggerAction)
		{
			if (!PlayerInput->IsPressed(EnhancedActionMapping.Key))
			{
				FName MappableName = EnhancedActionMapping.GetMappingName();
				if (!MappableName.IsNone())
				{
					RemovePlayerMappableNameInput(MappableName);
				}
			}
		}
	}
	RemoveInputActionTrigger(TriggerAction);
}