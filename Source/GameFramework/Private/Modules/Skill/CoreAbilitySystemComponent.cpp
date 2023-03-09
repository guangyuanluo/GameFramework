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
#include "SkillGroupConfigTableRow.h"
#include "SkillConfigTableRow.h"
#include "EffectConfigTableRow.h"
#include "CoreAbility.h"
#include "CoreAbilityComboExecutor.h"
#include "CoreAbilityCondition.h"
#include "SortUtils.h"
#include "FindEnemyComponent.h"

void UCoreAbilitySystemComponent::InitSkillFromTemplate(int TemplateId) {
    auto Owner = GetOwner();
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
            AddSkillGroup(FindTemplate->SkillGroupID, FEffectPreAddDelegate());
        }
    }
}

void UCoreAbilitySystemComponent::K2_AddSkillGroup(int SkillGroupID, const FEffectPreAddDynDelegate& InEffectPreAddCallback) {
    AddSkillGroup(SkillGroupID,  FEffectPreAddDelegate::CreateUFunction(const_cast<UObject*>(InEffectPreAddCallback.GetUObject()), InEffectPreAddCallback.GetFunctionName()));
}

void UCoreAbilitySystemComponent::AddSkillGroup(int SkillGroupID, const FEffectPreAddDelegate& InEffectPreAddCallback) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillGroupDataTable = SkillSetting->SkillGroupTable.LoadSynchronous();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();
    if (SkillGroupDataTable && SkillDataTable && EffectDataTable) {
        FSkillGroupConfigTableRow* FindSkillGroup = (FSkillGroupConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillGroupDataTable, SkillGroupID);
        if (!FindSkillGroup) {
            return;
        }

        auto Owner = GetOwner();

        //添加技能
        for (int Index = 0; Index < FindSkillGroup->Skills.Num(); ++Index) {
            AddSkillPrivate(SkillDataTable, FindSkillGroup->Skills[Index]);
        }
        //添加效果
        for (int Index = 0; Index < FindSkillGroup->Effects.Num(); ++Index) {
            AddEffectPrivate(EffectDataTable, FindSkillGroup->Effects[Index], InEffectPreAddCallback);
        }
    }
}

void UCoreAbilitySystemComponent::RemoveSkillGroup(int SkillGroupID) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillGroupDataTable = SkillSetting->SkillGroupTable.LoadSynchronous();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();
    if (SkillGroupDataTable && SkillDataTable && EffectDataTable) {
        FSkillGroupConfigTableRow* FindSkillGroup = (FSkillGroupConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillGroupDataTable, SkillGroupID);
        if (!FindSkillGroup) {
            return;
        }

        auto Owner = GetOwner();

        //移除技能
        for (int Index = 0; Index < FindSkillGroup->Skills.Num(); ++Index) {
            RemoveSkillPrivate(SkillDataTable, FindSkillGroup->Skills[Index]);
        }
        //移除效果
        for (int Index = 0; Index < FindSkillGroup->Effects.Num(); ++Index) {
            RemoveEffectPrivate(EffectDataTable, FindSkillGroup->Effects[Index]);
        }
    }
}

void UCoreAbilitySystemComponent::AddSkill(const FSkillInfo& SkillInfo) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    if (SkillDataTable) {
        AddSkillPrivate(SkillDataTable, SkillInfo);
    }
}

void UCoreAbilitySystemComponent::RemoveSkill(const FSkillInfo& SkillInfo) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    if (SkillDataTable) {
        RemoveSkillPrivate(SkillDataTable, SkillInfo);
    }
}

void UCoreAbilitySystemComponent::K2_AddEffect(const FEffectInfo& EffectInfo, const FEffectPreAddDynDelegate& InEffectPreAddCallback) {
    AddEffect(EffectInfo, FEffectPreAddDelegate::CreateUFunction(const_cast<UObject*>(InEffectPreAddCallback.GetUObject()), InEffectPreAddCallback.GetFunctionName()));
}

void UCoreAbilitySystemComponent::AddEffect(const FEffectInfo& EffectInfo, const FEffectPreAddDelegate& InPreAddCallback) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();
    if (EffectDataTable) {
        AddEffectPrivate(EffectDataTable, EffectInfo, InPreAddCallback);
    }
}

void UCoreAbilitySystemComponent::RemoveEffect(const FEffectInfo& EffectInfo) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto EffectDataTable = SkillSetting->EffectTable.LoadSynchronous();
    if (EffectDataTable) {
        RemoveEffectPrivate(EffectDataTable, EffectInfo);
    }
}

void UCoreAbilitySystemComponent::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfy, bool SortByOrder) {
    TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate, false);

    ACoreCharacter* TargetEnemy = nullptr;
    FGameplayTagContainer TargetTagContainer;
    if (AActor* ThisAvatarActor = GetAvatarActor()) {
        auto FindEnemyComponent = Cast<UFindEnemyComponent>(ThisAvatarActor->GetComponentByClass(UFindEnemyComponent::StaticClass()));
        if (FindEnemyComponent) {
            TargetEnemy = FindEnemyComponent->FindOrGetEnemy();
            if (TargetEnemy) {
                auto TargetAbilitySystemComponent = TargetEnemy->GetAbilitySystemComponent();
                if (TargetAbilitySystemComponent) {
                    TargetAbilitySystemComponent->GetOwnedGameplayTags(TargetTagContainer);
                }
            }
        }
    }

    // Iterate the list of all ability specs
    for (FGameplayAbilitySpec* Spec : AbilitiesToActivate) {
        // Iterate all instances on this ability spec
        TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

        for (UGameplayAbility* AbilityInstance : AbilityInstances) {
            UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
            if ((!ForceFilterActive || ActiveAbility->IsActive())
                && (!bOnlyAbilitiesThatSatisfy || ActiveAbility->DoesAbilitySatisfyTagRequirements(*this, nullptr, &TargetTagContainer))
                && (!bOnlyAbilitiesThatSatisfy || ActiveAbility->K2_IsConditionSatisfy())) {
                ActiveAbilities.Add(ActiveAbility);
            }
        }
    }
    if (SortByOrder && ActiveAbilities.Num() > 1) {
        TFunction<bool(UGameplayAbility*, UGameplayAbility*)> CompareFunc = [](UGameplayAbility* A, UGameplayAbility* B) {
            auto AbilityA = Cast<UCoreAbility>(A);
            auto AbilityB = Cast<UCoreAbility>(B);
            return AbilityA->SortPriority < AbilityB->SortPriority;
        };
        USortUtils::SortArray(ActiveAbilities, CompareFunc);
    }
}

void UCoreAbilitySystemComponent::GetActiveAbilitiesWithClass(TSubclassOf<UGameplayAbility> AbilityClass, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfy, bool SortByOrder) {
    auto FindSpec = FindAbilitySpecFromClass(AbilityClass);
    if (FindSpec) {
        TArray<UGameplayAbility*> AbilityInstances = FindSpec->GetAbilityInstances();

        ACoreCharacter* TargetEnemy = nullptr;
        FGameplayTagContainer TargetTagContainer;
        if (AActor* ThisAvatarActor = GetAvatarActor()) {
            auto FindEnemyComponent = Cast<UFindEnemyComponent>(ThisAvatarActor->GetComponentByClass(UFindEnemyComponent::StaticClass()));
            if (FindEnemyComponent) {
                TargetEnemy = FindEnemyComponent->FindOrGetEnemy();
                if (TargetEnemy) {
                    auto TargetAbilitySystemComponent = TargetEnemy->GetAbilitySystemComponent();
                    if (TargetAbilitySystemComponent) {
                        TargetAbilitySystemComponent->GetOwnedGameplayTags(TargetTagContainer);
                    }
                }
            }
        }

        for (UGameplayAbility* AbilityInstance : AbilityInstances) {
            UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
            if ((!ForceFilterActive || ActiveAbility->IsActive())
                && (!bOnlyAbilitiesThatSatisfy || ActiveAbility->DoesAbilitySatisfyTagRequirements(*this, nullptr, &TargetTagContainer))
                && (!bOnlyAbilitiesThatSatisfy || ActiveAbility->K2_IsConditionSatisfy())) {
                ActiveAbilities.Add(ActiveAbility);
            }
        }
        if (SortByOrder && ActiveAbilities.Num() > 1) {
            TFunction<bool(UGameplayAbility*, UGameplayAbility*)> CompareFunc = [](UGameplayAbility* A, UGameplayAbility* B) {
                auto AbilityA = Cast<UCoreAbility>(A);
                auto AbilityB = Cast<UCoreAbility>(B);
                return AbilityA->SortPriority < AbilityB->SortPriority;
            };
            USortUtils::SortArray(ActiveAbilities, CompareFunc);
        }
    }
}

void UCoreAbilitySystemComponent::GetActiveAbilitiesWithInputID(int32 InputID, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfy, bool SortByOrder) {
    ACoreCharacter* TargetEnemy = nullptr;
    FGameplayTagContainer TargetTagContainer;
    if (AActor* ThisAvatarActor = GetAvatarActor()) {
        auto FindEnemyComponent = Cast<UFindEnemyComponent>(ThisAvatarActor->GetComponentByClass(UFindEnemyComponent::StaticClass()));
        if (FindEnemyComponent) {
            TargetEnemy = FindEnemyComponent->FindOrGetEnemy();
            if (TargetEnemy) {
                auto TargetAbilitySystemComponent = TargetEnemy->GetAbilitySystemComponent();
                if (TargetAbilitySystemComponent) {
                    TargetAbilitySystemComponent->GetOwnedGameplayTags(TargetTagContainer);
                }
            }
        }
    }

    for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items) {
        if (Spec.InputID == InputID) {
            TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();

            for (UGameplayAbility* AbilityInstance : AbilityInstances) {
                UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
                if ((!ForceFilterActive || ActiveAbility->IsActive())
                    && (!bOnlyAbilitiesThatSatisfy || ActiveAbility->DoesAbilitySatisfyTagRequirements(*this, nullptr, &TargetTagContainer))
                    && (!bOnlyAbilitiesThatSatisfy || ActiveAbility->K2_IsConditionSatisfy())) {
                    ActiveAbilities.Add(ActiveAbility);
                }
            }
        }
    }
    if (SortByOrder && ActiveAbilities.Num() > 1) {
        TFunction<bool(UGameplayAbility*, UGameplayAbility*)> CompareFunc = [](UGameplayAbility* A, UGameplayAbility* B) {
            auto AbilityA = Cast<UCoreAbility>(A);
            auto AbilityB = Cast<UCoreAbility>(B);
            return AbilityA->SortPriority > AbilityB->SortPriority;
        };
        USortUtils::SortArray(ActiveAbilities, CompareFunc);
    }
}

void UCoreAbilitySystemComponent::ResetAbilityCooldown(UGameplayAbility* Ability) {
    auto CooldownEffect = Ability->GetCooldownGameplayEffect();
    if (CooldownEffect) {
        RemoveActiveGameplayEffectBySourceEffect(CooldownEffect->GetClass(), this);
    }
    Ability->K2_CommitAbilityCooldown();
}

void UCoreAbilitySystemComponent::TryComboAbilityByClass(UCoreAbility* Ability, FGameplayTag TriggerWayTag) {
    if (!Ability || !IsValid(Ability)) {
        UE_LOG(GameCore, Warning, TEXT("TryComboAbilityByClass called with invalid Ability"));
        return;
    }

    if (!Ability->IsActive()) {
        return;
    }

    const UGameplayAbility* const InAbilityCDO = Ability->GetClass()->GetDefaultObject<UGameplayAbility>();

    FGameplayAbilitySpecHandle AbilityToCombo;
    for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items) {
        if (Spec.Ability == InAbilityCDO) {
            AbilityToCombo = Spec.Handle;
            break;
        }
    }
    if (!AbilityToCombo.IsValid()) {
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
        auto MontageInstance = AnimInstance->GetActiveMontageInstance();
        if (!MontageInstance) {
            return;
        }
        FName CurrentSection = MontageInstance->GetCurrentSection();
        auto FindComboSectionConfigPtr = Ability->ComboMap.Find(CurrentSection);
        if (FindComboSectionConfigPtr) {
            const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
            bool HaveAnyComboEnable = false;

            //赋值上下文变量
            SetCurrentTriggerWayTag(TriggerWayTag);

            for (auto SectionConfig : FindComboSectionConfigPtr->Configs) {
                //赋值上下文变量
                CurrentCheckComboSection = &SectionConfig;

                bool CheckCombo = true;
                for (auto ComboCondition : SkillSetting->GlobalComboConditions) {
                    if (!ComboCondition.Get()) {
                        continue;
                    }
                    auto ComboConditionCDO = ComboCondition->GetDefaultObject<UCoreAbilityCondition>();
                    FLogicObjectLoadWorldScope LoadWorldScope(ComboConditionCDO, this);

                    if (!ComboConditionCDO->DoesSatisfy(this, Ability)) {
                        CheckCombo = false;
                        break;
                    }
                }
                if (!CheckCombo) {
                    continue;
                }
                for (auto ComboCondition : SectionConfig.Conditions) {
                    if (!ComboCondition.Get()) {
                        continue;
                    }
                    auto ComboConditionCDO = ComboCondition->GetDefaultObject<UCoreAbilityCondition>();
                    FLogicObjectLoadWorldScope LoadWorldScope(ComboConditionCDO, this);

                    if (!ComboConditionCDO->DoesSatisfy(this, Ability)) {
                        CheckCombo = false;
                        break;
                    }
                }
                if (!CheckCombo) {
                    continue;
                }
                HaveAnyComboEnable = true;
                break;
            }

            //清空上下文变量
            SetCurrentTriggerWayTag(FGameplayTag::EmptyTag);
            CurrentCheckComboSection = nullptr;

            if (!HaveAnyComboEnable) {
                return;
            }
        }
    }

    if (!bIsLocal && (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalOnly || Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)) {
        InternalComboAbility(Ability, TriggerWayTag);
        return;
    }
    if (NetMode != ROLE_Authority && (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::ServerOnly || Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::ServerInitiated)) {
        ServerTryComboAbility(AbilityToCombo, TriggerWayTag);
        return;
    }
    if (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalOnly || (NetMode == ROLE_Authority)) {
        InternalComboAbility(Ability, TriggerWayTag);
    }
    else if (Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted) {
        InternalComboAbility(Ability, TriggerWayTag);
    }
    if (NetMode != ROLE_Authority) {
        ServerTryComboAbility(AbilityToCombo, TriggerWayTag);
    }
}

void UCoreAbilitySystemComponent::ServerTryComboAbility_Implementation(FGameplayAbilitySpecHandle AbilityToCombo, FGameplayTag TriggerWayTag) {
    auto AbilitySpec = FindAbilitySpecFromHandle(AbilityToCombo);
    if (!AbilitySpec) {
        UE_LOG(GameCore, Warning, TEXT("TryComboAbilityByClass called fail:find ability spec fail"));
        return;
    }
    TArray<UGameplayAbility*> ActivateAbilities = AbilitySpec->GetAbilityInstances();
    if (ActivateAbilities.Num() == 0) {
        UE_LOG(GameCore, Warning, TEXT("TryComboAbilityByClass called with not active Ability"));
        return;
    }
    auto Ability = Cast<UCoreAbility>(ActivateAbilities[0]);
    InternalComboAbility(Ability, TriggerWayTag);
}

bool UCoreAbilitySystemComponent::ServerTryComboAbility_Validate(FGameplayAbilitySpecHandle AbilityToCombo, FGameplayTag TriggerWayTag) {
    return true;
}

void UCoreAbilitySystemComponent::K2_AbilityLocalInputPressed(int32 InputID) {
    AbilityLocalInputPressed(InputID);
}

void UCoreAbilitySystemComponent::K2_AbilityLocalInputReleased(int32 InputID) {
    AbilityLocalInputReleased(InputID);
}

FGameplayTag UCoreAbilitySystemComponent::GetCurrentTriggerWayTag() const {
    return CurrentTriggerWayTag;
}

void UCoreAbilitySystemComponent::SetCurrentTriggerWayTag(FGameplayTag TriggerWayTag) {
    CurrentTriggerWayTag = TriggerWayTag;
}

FComboSectionConfig* UCoreAbilitySystemComponent::GetCurrentCheckComboSection() const {
    return CurrentCheckComboSection;
}

void UCoreAbilitySystemComponent::InternalComboAbility(UCoreAbility* Ability, FGameplayTag TriggerWayTag) {
    if (!Ability || !Ability->IsActive()) {
        UE_LOG(GameCore, Warning, TEXT("TryComboAbilityByClass called with not active Ability"));
        return;
    }
    UAnimInstance* AnimInstance = AbilityActorInfo.IsValid() ? AbilityActorInfo->GetAnimInstance() : nullptr;
    if (LocalAnimMontageInfo.AnimMontage && AnimInstance && LocalAnimMontageInfo.AnimMontage == Ability->GetCurrentMontage()) {
        auto MontageInstance = AnimInstance->GetActiveMontageInstance();
        if (!MontageInstance) {
            return;
        }

        //赋值上下文变量
        SetCurrentTriggerWayTag(TriggerWayTag);

        FName CurrentSection = MontageInstance->GetCurrentSection();
        auto FindComboSectionConfigsPtr = Ability->ComboMap.Find(CurrentSection);
        if (FindComboSectionConfigsPtr) {
            const USkillSetting* SkillSetting = GetDefault<USkillSetting>();

            UCoreAbilityComboExecutor* FindComboExecutor = nullptr;

            for (auto SectionConfig : FindComboSectionConfigsPtr->Configs) {
                //赋值上下文变量
                CurrentCheckComboSection = &SectionConfig;

                bool CheckCombo = true;
                for (auto ComboCondition : SkillSetting->GlobalComboConditions) {
                    if (!ComboCondition.Get()) {
                        continue;
                    }
                    auto ComboConditionCDO = ComboCondition->GetDefaultObject<UCoreAbilityCondition>();
                    FLogicObjectLoadWorldScope LoadWorldScope(ComboConditionCDO, this);

                    if (!ComboConditionCDO->DoesSatisfy(this, Ability)) {
                        CheckCombo = false;
                        break;
                    }
                }
                if (!CheckCombo) {
                    continue;
                }
                for (auto ComboCondition : SectionConfig.Conditions) {
                    if (!ComboCondition.Get()) {
                        continue;
                    }
                    auto ComboConditionCDO = ComboCondition->GetDefaultObject<UCoreAbilityCondition>();
                    FLogicObjectLoadWorldScope LoadWorldScope(ComboConditionCDO, this);

                    if (!ComboConditionCDO->DoesSatisfy(this, Ability)) {
                        CheckCombo = false;
                        break;
                    }
                }
                if (!CheckCombo) {
                    continue;
                }

                //所有条件检查完了，执行
                check(Ability->ComboExecutor);
                FindComboExecutor = Ability->ComboExecutor->GetDefaultObject<UCoreAbilityComboExecutor>();
                break;
            }

            if (FindComboExecutor) {
                FLogicObjectLoadWorldScope LoadWorldScope(FindComboExecutor, this);
                FindComboExecutor->ExecuteCombo(this, Ability, *CurrentCheckComboSection, TriggerWayTag);
            }

            //清空上下文变量
            SetCurrentTriggerWayTag(FGameplayTag::EmptyTag);
            CurrentCheckComboSection = nullptr;
        }
    }
}

void UCoreAbilitySystemComponent::AddSkillPrivate(class UDataTable* SkillDataTable, const FSkillInfo& SkillInfo) {
    if (GetOwner()->GetLocalRole() != ROLE_Authority) {
        return;
    }
    auto FindSkill = (FSkillConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, SkillInfo.SkillID);
    if (FindSkill) {
        if (FindSkill->GameplayAbilityClass) {
            GiveAbility(FGameplayAbilitySpec(FindSkill->GameplayAbilityClass, SkillInfo.SkillLevel, FindSkill->InputID, GetOwner()));
        }
        else {
            UE_LOG(GameCore, Warning, TEXT("技能模板组配置的技能有空类，请检查配置，技能id:%d"), SkillInfo.SkillID);
        }
    }
    else {
        UE_LOG(GameCore, Warning, TEXT("技能模板组配置的技能找不到:%d"), SkillInfo.SkillID);
    }
}

void UCoreAbilitySystemComponent::RemoveSkillPrivate(class UDataTable* SkillDataTable, const FSkillInfo& SkillInfo) {    
    if (GetOwner()->GetLocalRole() != ROLE_Authority) {
        return;
    }
    auto FindSkill = (FSkillConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, SkillInfo.SkillID);
    if (FindSkill) {
        if (FindSkill->GameplayAbilityClass) {
            auto FindAbilitySpec = FindAbilitySpecFromClass(FindSkill->GameplayAbilityClass);
            if (FindAbilitySpec) {
                ClearAbility(FindAbilitySpec->Handle);
            }
        }
        else {
            UE_LOG(GameCore, Warning, TEXT("技能模板组配置的技能有空类，请检查配置，技能id:%d"), SkillInfo.SkillID);
        }
    }
    else {
        UE_LOG(GameCore, Warning, TEXT("技能模板组配置的技能找不到:%d"), SkillInfo.SkillID);
    }
}

void UCoreAbilitySystemComponent::AddEffectPrivate(class UDataTable* EffectDataTable, const FEffectInfo& EffectInfo, const FEffectPreAddDelegate& InPreAddCallback) {
    if (GetOwner()->GetLocalRole() != ROLE_Authority) {
        return;
    }
    auto FindEffect = (FEffectConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(EffectDataTable, EffectInfo.EffectID);
    if (FindEffect) {
        if (FindEffect->GameplayEffectClass) {
            FGameplayEffectContextHandle EffectContext = MakeEffectContext();
            FGameplayEffectSpecHandle NewHandle = MakeOutgoingSpec(FindEffect->GameplayEffectClass, EffectInfo.EffectLevel, EffectContext);
            if (NewHandle.IsValid()) {
                EffectPreAddDynMutiDelegate.Broadcast(this, NewHandle, FindEffect->GameplayEffectClass);
                InPreAddCallback.ExecuteIfBound(this, NewHandle, FindEffect->GameplayEffectClass);
                ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
            }
        }
        else {
            UE_LOG(GameCore, Warning, TEXT("技能模板组配置的效果有空类，请检查配置，效果id:%d"), EffectInfo.EffectID);
        }
    }
    else {
        UE_LOG(GameCore, Warning, TEXT("技能模板组配置的效果找不到:%d"), EffectInfo.EffectID);
    }
}

void UCoreAbilitySystemComponent::RemoveEffectPrivate(class UDataTable* EffectDataTable, const FEffectInfo& EffectInfo) {
    if (GetOwner()->GetLocalRole() != ROLE_Authority) {
        return;
    }
    auto FindEffect = (FEffectConfigTableRow*)UConfigTableCache::GetDataTableRawDataFromId(EffectDataTable, EffectInfo.EffectID);
    if (FindEffect) {
        if (FindEffect->GameplayEffectClass) {
            RemoveActiveGameplayEffectBySourceEffect(FindEffect->GameplayEffectClass, this, -1);
        }
        else {
            UE_LOG(GameCore, Warning, TEXT("技能模板组配置的效果有空类，请检查配置，效果id:%d"), EffectInfo.EffectID);
        }
    }
    else {
        UE_LOG(GameCore, Warning, TEXT("技能模板组配置的效果找不到:%d"), EffectInfo.EffectID);
    }
}