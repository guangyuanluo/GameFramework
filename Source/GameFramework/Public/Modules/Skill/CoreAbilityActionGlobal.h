// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/TriggerAction/CoreTriggerAction.h"
#include "CoreAbilityActionGlobal.generated.h"

class UCoreAbility;

UCLASS(meta = (DisplayName = "变更技能剩余计数", ShowDisplayNames))
class GAMEFRAMEWORK_API UAbilityTriggerAction_ChangeRestCounter : public UCoreTriggerAction {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int ChangeCounter = -1;

    virtual void OnExecute_Implementation(const FGameplayEventData& EventData) const override;
};