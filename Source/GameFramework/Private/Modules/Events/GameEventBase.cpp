// Fill out your copyright notice in the Description page of Project Settings.

#include "Modules/Events/GameEventBase.h"
#include "PlayerController/CorePlayerController.h"

class ACorePlayerController* UGameEventBase::GetOwnerPlayerController() const {
    return Cast<ACorePlayerController>(GetOuter());
}