// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEventBase.h"
#include "CorePlayerController.h"

class ACorePlayerController* UGameEventBase::GetOwnerPlayerController() const {
    return Cast<ACorePlayerController>(GetOuter());
}