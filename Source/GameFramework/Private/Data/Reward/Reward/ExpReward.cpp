// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpReward.h"
#include "CoreCharacter.h"
#include "UnitSetting.h"
#include "ConfigTableCache.h"
#include "UnitInfoConfigTableRow.h"
#include "GameEntity.h"

UExpReward::UExpReward(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	RewardType = RewardType::E_Exp;
}

void UExpReward::HandleRewardDispatch(UCoreGameInstance* gameInstance, TScriptInterface<class IGameEntity> target) {
	auto Character = Cast<ACoreCharacter>(target.GetObject());
	if (Character != nullptr) {
        const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
        auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
        if (UnitDataTable) {
            auto UnitTemplate = (FUnitInfoConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(UnitDataTable, Character->TemplateID);
            if (UnitTemplate != nullptr) {
                if (UnitTemplate->GrowExpTypeId == ExpType) {
                    //Character->AddLevelExp(Count);
                    return;
                }
            }
        }
	}
	//todo
}