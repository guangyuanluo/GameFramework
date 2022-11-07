// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreOrderBase.h"

UCoreOrderBase::UCoreOrderBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
    if (!ID.IsValid()) {
        ID = FGuid::NewGuid();
    }
}