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
#include "CoreAbility.h"
#include "CoreAbilityComboExecutor.h"

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

void UCoreAbilitySystemComponent::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfyTagRequirements) {
    TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate, false);

    // Iterate the list of all ability specs
    for (FGameplayAbilitySpec* Spec : AbilitiesToActivate) {
        // Iterate all instances on this ability spec
        TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

        for (UGameplayAbility* ActiveAbility : AbilityInstances) {
            if ((!ForceFilterActive || ActiveAbility->IsActive())
                && (!bOnlyAbilitiesThatSatisfyTagRequirements || ActiveAbility->DoesAbilitySatisfyTagRequirements(*this))) {
                ActiveAbilities.Add(ActiveAbility);
            }
        }
    }
}

void UCoreAbilitySystemComponent::GetActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> AbilityClass, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfyTagRequirements) {
    auto FindSpec = FindAbilitySpecFromClass(AbilityClass);
    if (FindSpec) {
        TArray<UGameplayAbility*> AbilityInstances = FindSpec->GetAbilityInstances();

        for (UGameplayAbility* ActiveAbility : AbilityInstances) {
            if ((!ForceFilterActive || ActiveAbility->IsActive())
                && (!bOnlyAbilitiesThatSatisfyTagRequirements || ActiveAbility->DoesAbilitySatisfyTagRequirements(*this))) {
                ActiveAbilities.Add(ActiveAbility);
            }
        }
    }
}

void UCoreAbilitySystemComponent::ResetAbilityCooldown(UGameplayAbility* Ability) {
    auto CooldownEffect = Ability->GetCooldownGameplayEffect();
    if (CooldownEffect) {
        RemoveActiveGameplayEffectBySourceEffect(CooldownEffect->GetClass(), this);
    }
}

void UCoreAbilitySystemComponent::TryComboAbilityByClass(UCoreAbility* Ability) {
    if (!Ability || !IsValid(Ability)) {
        ABILITY_LOG(Warning, TEXT("TryComboAbilityByClass called with invalid Ability"));
        return;
    }

    if (!Ability->IsActive()) {
        return;
    }

    const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();

    // make sure the ActorInfo and then Actor on that FGameplayAbilityActorInfo are valid, if not bail out.
    if (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid()) {
        return;
    }


    const ENetRole NetMode = ActorInfo->AvatarActor->GetLocalRole();

    // This should only come from button presses/local instigation (AI, etc).
    if (NetMode == ROLE_SimulatedProxy) {
        return;
    }

    bool bIsLocal = AbilityActorInfo->IsLocallyControlled();

    UAnimInstance* AnimInstance = AbilityActorInfo.IsValid() ? AbilityActorInfo->GetAnimInstance() : nullptr;
    if (LocalAnimMontageInfo.AnimMontage && AnimInstance && LocalAnimMontageInfo.AnimMontage == Ability->GetCurrentMontage()) {
        FName CurrentSection = AnimInstance->GetActiveMontageInstance()->GetCurrentSection();
        auto FindExecutor = Ability->ComboMap.Find(CurrentSection);
        if (FindExecutor && FindExecutor->Get()) {
            auto Executor = FindExecutor->GetDefaultObject();
            if (!Executor->CheckComboEnable(Ability)) {
                return;
            }
        }
    }

    if (!bIsLocal && (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalOnly || Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)) {
        InternalComboAbility(Ability);
        return;
    }
    if (NetMode != ROLE_Authority && (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::ServerOnly || Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::ServerInitiated)) {
        ServerTryComboAbility(Ability);
        return;
    }
    InternalComboAbility(Ability);
}

void UCoreAbilitySystemComponent::ServerTryComboAbility_Implementation(UCoreAbility* Ability) {
    InternalComboAbility(Ability);
}

bool UCoreAbilitySystemComponent::ServerTryComboAbility_Validate(UCoreAbility* Ability) {
    return true;
}

void UCoreAbilitySystemComponent::InternalComboAbility(UCoreAbility* Ability) {
    if (!Ability->IsActive()) {
        ABILITY_LOG(Warning, TEXT("TryComboAbilityByClass called with not active Ability"));
        return;
    }
    UAnimInstance* AnimInstance = AbilityActorInfo.IsValid() ? AbilityActorInfo->GetAnimInstance() : nullptr;
    if (LocalAnimMontageInfo.AnimMontage && AnimInstance && LocalAnimMontageInfo.AnimMontage == Ability->GetCurrentMontage()) {
        FName CurrentSection = AnimInstance->GetActiveMontageInstance()->GetCurrentSection();
        auto FindExecutor = Ability->ComboMap.Find(CurrentSection);
        if (FindExecutor && FindExecutor->Get()) {
            auto Executor = FindExecutor->GetDefaultObject();
            if (!Executor->CheckComboEnable(Ability)) {
                return;
            }
            Executor->ExecuteCombo(Ability);
            Ability->NotifyComboAbility(CurrentSection);
        }
    }
}