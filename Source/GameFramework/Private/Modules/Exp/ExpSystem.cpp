// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpSystem.h"
#include "ExpComponent.h"
#include "CoreGameInstance.h"
#include "GameEntity.h"
#include "GameEntityManager.h"
#include "GameFrameworkUtils.h"
#include "ConfigTableCache.h"
#include "ExpSetting.h"
#include "UE4LogImpl.h"
#include "CoreCharacterStateBase.h"
#include "EventSystem.h"
#include "GameSystemManager.h"
#include "ExpLevelUpEvent.h"
#include "CoreCharacter.h"
#include "ExpTypeConfigTableRow.h"

void UExpSystem::Initialize(UCoreGameInstance* InGameInstance) {
    Super::Initialize(InGameInstance);

    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->RegistEventHandler(this);
}
void UExpSystem::Uninitialize() {
    Super::Uninitialize();
}

bool UExpSystem::AddExp(UExpComponent* ExpComponent, EExpTypeEnum ExpType, int32 Exp, const FString& Reason, FString& Error) {
    bool Result = false;
    auto FindIndex = -1;
    for (int Index = 0; Index < ExpComponent->Exps.Num(); ++Index) {
        if (ExpComponent->Exps[Index].ExpType == ExpType) {
            FindIndex = Index;
            break;
        }
    }
    if (FindIndex != -1) {
        const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
        auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();
        if (ExpTypeDataTable) {
            UEventSystem* EventSystem = GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>();

            FExpInfo OldExp = ExpComponent->Exps[FindIndex];
            Result = AddExpPrivate(ExpTypeDataTable, ExpComponent, FindIndex, ExpType, Exp, Reason, Error);
            FExpInfo NewExp = ExpComponent->Exps[FindIndex];
            if (OldExp.Level != NewExp.Level) {
                UExpLevelUpEvent* ExpLevelUpEvent = NewObject<UExpLevelUpEvent>();
                ExpLevelUpEvent->Source = UGameFrameworkUtils::GetCharacterFromComponentOwner(ExpComponent);
                ExpLevelUpEvent->ExpType = ExpType;
                ExpLevelUpEvent->ExpLevel = NewExp.Level;

                EventSystem->PushEvent(ExpLevelUpEvent);
            }
        }
        else {
            Error = TEXT("增加经验失败");
            UE_LOG(GameCore, Warning, TEXT("没有配置经验类型表，无法增加经验"));
        }
    }
    else {
        Error = TEXT("增加经验失败");
        UE_LOG(GameCore, Warning, TEXT("增加经验失败，找不到对应经验:%d"), ExpType);
    }
    return Result;
}

int UExpSystem::GetLevelUpgradeExp(EExpTypeEnum ExpType, int32 Level) {
    int MaxExp = 0;

    const UExpSetting* ExpSetting = GetDefault<UExpSetting>();
    auto ExpTypeDataTable = ExpSetting->ExpTypeTable.LoadSynchronous();
    if (ExpTypeDataTable) {
        auto FindExpType = (FExpTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ExpTypeDataTable, (int)ExpType);
        if (FindExpType) {
            if (!FindExpType->ExpLevelTable.GetLongPackageName().IsEmpty()) {
                UCurveTable* ExpLevelTable = FindExpType->ExpLevelTable.LoadSynchronous();
                if (ExpLevelTable) {
                    static const FName UpgradeExpValueName(TEXT("UpgradeExpValue"));
                    if (FRealCurve* UpgradeExpValueCurve = ExpLevelTable->FindCurve(UpgradeExpValueName, FString(), false)) {
                        MaxExp = UpgradeExpValueCurve->Eval(Level);
                    }
                }
            }
        }
    }
    else {
        UE_LOG(GameCore, Warning, TEXT("没有配置经验类型表，无法增加经验"));
    }

    return MaxExp;
}

bool UExpSystem::AddExpPrivate(class UDataTable* ExpTypeDataTable, UExpComponent* ExpComponent, int ExpIndex, EExpTypeEnum ExpType, int32 Exp, const FString& Reason, FString& Error) {
    if (Exp > 0) {
        //这里是增加经验
        auto FindExpType = (FExpTypeConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(ExpTypeDataTable, (int)ExpType);
        if (FindExpType) {
            if (!FindExpType->ExpLevelTable.GetLongPackageName().IsEmpty()) {
                UCurveTable* ExpLevelTable = FindExpType->ExpLevelTable.LoadSynchronous();
                if (ExpLevelTable) {
                    int UpgradeExpValue = 0;	//升级所需经验
                    int MaxAccumulatedExp = 0;	//最大累积经验
                    bool AutoUpgrade = false;	//是否自动升级

                    static const FName MaxAccumulatedExpName(TEXT("MaxAccumulatedExp"));
                    if (FRealCurve* MaxAccumulatedExpCurve = ExpLevelTable->FindCurve(MaxAccumulatedExpName, FString(), false)) {
                        MaxAccumulatedExp = MaxAccumulatedExpCurve->Eval(ExpComponent->Exps[ExpIndex].Level);

                        static const FName UpgradeExpValueName(TEXT("UpgradeExpValue"));
                        if (FRealCurve* UpgradeExpValueCurve = ExpLevelTable->FindCurve(UpgradeExpValueName, FString(), false)) {
                            UpgradeExpValue = UpgradeExpValueCurve->Eval(ExpComponent->Exps[ExpIndex].Level);

                            static const FName AutoUpgradeName(TEXT("AutoUpgrade"));
                            if (FRealCurve* AutoUpgradeCurve = ExpLevelTable->FindCurve(AutoUpgradeName, FString(), false)) {
                                AutoUpgrade = (bool)AutoUpgradeCurve->Eval(ExpComponent->Exps[ExpIndex].Level);

                                auto OldExp = ExpComponent->Exps[ExpIndex].Exp;
                                auto NewExp = FMath::Min(ExpComponent->Exps[ExpIndex].Exp + Exp, UpgradeExpValue);
                                auto DiffExp = NewExp - OldExp;
                                if (DiffExp != 0) {
                                    ExpComponent->Exps[ExpIndex].Exp = NewExp;
                                    if (ExpComponent->Exps[ExpIndex].Exp >= UpgradeExpValue) {
                                        if (AutoUpgrade) {
                                            //自动升级
                                            ExpComponent->Exps[ExpIndex].Level += 1;
                                            ExpComponent->Exps[ExpIndex].Exp = 0;

                                            UE_LOG(GameCore, Log, TEXT("AddExp:%d, upgrade level, new level:%d"), DiffExp, ExpComponent->Exps[ExpIndex].Level);
                                            //看是否连续升级
                                            if (DiffExp < Exp) {
                                                AddExpPrivate(ExpTypeDataTable, ExpComponent, ExpIndex, ExpType, Exp - DiffExp, Reason, Error);
                                            }
                                        }
                                        else {
                                            UE_LOG(GameCore, Log, TEXT("AddExp:%d"), DiffExp);

                                            //不超过最大累积经验
                                            ExpComponent->Exps[ExpIndex].Exp = FMath::Min(ExpComponent->Exps[ExpIndex].Exp, UpgradeExpValue + MaxAccumulatedExp);
                                        }
                                    }
                                    else {
                                        UE_LOG(GameCore, Log, TEXT("AddExp:%d"), DiffExp);
                                    }

                                    ExpComponent->OnExpChanged();

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Error = TEXT("增加经验失败");
    UE_LOG(GameCore, Warning, TEXT("增加经验失败"));

    return false;
}

TArray<TSubclassOf<class UGameEventBase>> UExpSystem::GetHandleEventTypes() {
    return {
		UAddExpRequesEvent::StaticClass(),
	};
}

void UExpSystem::OnEvent(UCoreGameInstance* InGameInstance, UGameEventBase* HandleEvent) {
	if (HandleEvent->IsA(UAddExpRequesEvent::StaticClass())) {
		auto Request = Cast<UAddExpRequesEvent>(HandleEvent);

		auto Entity = InGameInstance->GameEntityManager->GetEntityById(Request->EntityId);
		if (Entity) {
            auto CharacterState = UGameFrameworkUtils::GetEntityState(Entity);
			if (CharacterState && CharacterState->ExpComponent) {
				FString Error;
				AddExp(CharacterState->ExpComponent, Request->ExpType, Request->Exp, Request->Reason, Error);
			}
		}
	}
}