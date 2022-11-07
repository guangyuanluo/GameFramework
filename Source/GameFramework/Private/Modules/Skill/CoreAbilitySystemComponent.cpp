// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoreAbilitySystemComponent.h"
#include "UE4LogImpl.h"
#include "ExpComponent.h"
#include "CoreCharacterStateBase.h"
#include "CoreCharacter.h"
#include "UnitSetting.h"
#include "SkillSetting.h"
#include "ConfigTableCache.h"
#include "UnitInfoConfigTableRow.h"
#include "SkillConfigTableRow.h"

void UCoreAbilitySystemComponent::InitSkillFromTemplate(int TemplateId) {
    auto Owner = Cast<AActor>(GetOwner());
    if (Owner->GetLocalRole() == ROLE_Authority) {
        ClearAllAbilities();
        FGameplayEffectQuery QueryAllEffects;
        QueryAllEffects.CustomMatchDelegate.BindLambda([](const FActiveGameplayEffect& MatchEffect) { return true; });
        RemoveActiveEffects(QueryAllEffects, -1);
    }

    const UUnitSetting* UnitSetting = GetDefault<UUnitSetting>();
    auto UnitDataTable = UnitSetting->UnitTable.LoadSynchronous();
    if (UnitDataTable) {
        auto FindTemplate = (FUnitInfoConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(UnitDataTable, TemplateId);
        if (FindTemplate) {
            //初始化属性
            if (!FindTemplate->AttributeSetClass) {
                UE_LOG(GameCore, Error, TEXT("使用技能系统，但是属性集类没设置，模板id:%d"), TemplateId);
                return;
            }
            InitStats(FindTemplate->AttributeSetClass, nullptr);
            //从属性表初始化属性
            if (FindTemplate->AttributeEffectClass && !FindTemplate->AttributeTable.GetLongPackageName().IsEmpty()) {
                auto CharacterState = Cast<ACoreCharacterStateBase>(Owner);
                if (ensureMsgf(CharacterState, TEXT("Character没有CharacterState对象"))) {
                    if (Owner->GetLocalRole() == ROLE_Authority) {
                        auto ExpLevel = CharacterState->ExpComponent->GetExpLevel(FindTemplate->GrowExpTypeId);

                        FGameplayEffectContextHandle EffectContext = MakeEffectContext();
                        FGameplayEffectSpecHandle NewHandle = MakeOutgoingSpec(FindTemplate->AttributeEffectClass, ExpLevel, EffectContext);
                        if (NewHandle.IsValid()) {
                            ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
                        }
                    }
                }
            }
            const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
            auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
            if (SkillDataTable) {
                //添加普通攻击技能
                auto ActivateSkill = (FSkillConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, FindTemplate->ActivateSkill.SkillId);
                if (ActivateSkill && ActivateSkill->GameplayAbilityClass) {
                    if (Owner->GetLocalRole() == ROLE_Authority) {
                        GiveAbility(FGameplayAbilitySpec(ActivateSkill->GameplayAbilityClass, FindTemplate->ActivateSkill.SkillLevel, INDEX_NONE, Owner));
                    }
                }
                else {
                    UE_LOG(GameCore, Warning, TEXT("使用技能系统，但是单位没设置普攻技能，模板id:%d"), TemplateId);
                }
                //添加主动技能
                for (int Index = 0; Index < FindTemplate->InitSkills.Num(); ++Index) {
                    auto InitSkill = (FSkillConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, FindTemplate->InitSkills[Index].SkillId);
                    if (InitSkill && InitSkill->GameplayAbilityClass) {
                        if (Owner->GetLocalRole() == ROLE_Authority) {
                            GiveAbility(FGameplayAbilitySpec(InitSkill->GameplayAbilityClass, FindTemplate->InitSkills[Index].SkillLevel, INDEX_NONE, Owner));
                        }
                    }
                    else {
                        UE_LOG(GameCore, Warning, TEXT("单位配置的主动技能中有空类的，请检查配置，模板id:%d"), TemplateId);
                    }
                }
                //添加被动效果
                for (int Index = 0; Index < FindTemplate->InitPassiveEffects.Num(); ++Index) {
                    if (FindTemplate->InitPassiveEffects[Index].GameplayEffectClass) {
                        if (Owner->GetLocalRole() == ROLE_Authority) {
                            FGameplayEffectContextHandle EffectContext = MakeEffectContext();
                            FGameplayEffectSpecHandle NewHandle = MakeOutgoingSpec(FindTemplate->InitPassiveEffects[Index].GameplayEffectClass, FindTemplate->InitPassiveEffects[Index].EffectLevel, EffectContext);
                            if (NewHandle.IsValid()) {
                                ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
                            }
                        }
                    }
                    else {
                        UE_LOG(GameCore, Warning, TEXT("单位配置的被动效果中有空类的，请检查配置，模板id:%d"), TemplateId);
                    }
                }
            }
        }
    }
}