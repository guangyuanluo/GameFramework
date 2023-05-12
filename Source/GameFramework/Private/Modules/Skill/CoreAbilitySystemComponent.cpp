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

bool UCoreAbilitySystemComponent::DoesAbilityFilterCondition(const FGameplayTagContainer& TargetTagContainer, UCoreAbility* ActiveAbility, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfy) {
    if (ForceFilterActive) {
        if (!ActiveAbility->IsActive()) {
            return false;
        }
    }
    else {
        if (bOnlyAbilitiesThatSatisfy
            && (
                !ActiveAbility->DoesAbilitySatisfyTagRequirements(*this, nullptr, &TargetTagContainer)
                || !ActiveAbility->K2_IsConditionSatisfy()
                )
            ) {
            return false;
        }
    }
    return true;
}

void UCoreAbilitySystemComponent::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfy, bool SortByOrder) {
    TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToActivate, false);

    FGameplayTagContainer TargetTagContainer = GetTargetTagContainer();

    // Iterate the list of all ability specs
    for (FGameplayAbilitySpec* Spec : AbilitiesToActivate) {
        // Iterate all instances on this ability spec
        TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

        for (UGameplayAbility* AbilityInstance : AbilityInstances) {
            UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
            if (ActiveAbility && DoesAbilityFilterCondition(TargetTagContainer, ActiveAbility, ForceFilterActive, bOnlyAbilitiesThatSatisfy)) {
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

        FGameplayTagContainer TargetTagContainer = GetTargetTagContainer();

        for (UGameplayAbility* AbilityInstance : AbilityInstances) {
            UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
            if (ActiveAbility && DoesAbilityFilterCondition(TargetTagContainer, ActiveAbility, ForceFilterActive, bOnlyAbilitiesThatSatisfy)) {
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
    FGameplayTagContainer TargetTagContainer = GetTargetTagContainer();

    for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items) {
        if (Spec.InputID == InputID) {
            TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();

            for (UGameplayAbility* AbilityInstance : AbilityInstances) {
                UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
                if (ActiveAbility && DoesAbilityFilterCondition(TargetTagContainer, ActiveAbility, ForceFilterActive, bOnlyAbilitiesThatSatisfy)) {
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

TSubclassOf<UCoreAbility> UCoreAbilitySystemComponent::GetActiveAbilityWithInputID(int32 InputID, bool ForceFilterActive, bool bOnlyAbilitiesThatSatisfy, bool SortByOrder) {
    FGameplayTagContainer TargetTagContainer = GetTargetTagContainer();

    for (const auto& ActiveAbility : SortCoreAbilities) {
        auto AbilitySpec = ActiveAbility->GetCurrentAbilitySpec();
        if (AbilitySpec->InputID == InputID) {
            if (DoesAbilityFilterCondition(TargetTagContainer, ActiveAbility, ForceFilterActive, bOnlyAbilitiesThatSatisfy)) {
                return ActiveAbility->GetClass();
            }
        }
    }
    return nullptr;
}

void UCoreAbilitySystemComponent::ResetAbilityCooldown(UGameplayAbility* Ability) {
    auto CooldownEffect = Ability->GetCooldownGameplayEffect();
    if (CooldownEffect) {
        RemoveActiveGameplayEffectBySourceEffect(CooldownEffect->GetClass(), this);
    }
    Ability->K2_CommitAbilityCooldown();
}

void UCoreAbilitySystemComponent::ClearAbilityCooldown(UGameplayAbility* Ability) {
    auto CooldownEffect = Ability->GetCooldownGameplayEffect();
    if (CooldownEffect) {
        RemoveActiveGameplayEffectBySourceEffect(CooldownEffect->GetClass(), this);
    }
}

void UCoreAbilitySystemComponent::K2_AbilityLocalInputPressed(int32 InputID) {
    InputPressedLocal(InputID);

    auto NetMode = GetWorld()->GetNetMode();
    if (NetMode == ENetMode::NM_Client) {
        ServerInputPressed(InputID);
    }
}

void UCoreAbilitySystemComponent::K2_AbilityLocalInputReleased(int32 InputID) {
    InputReleasedLocal(InputID);

    auto NetMode = GetWorld()->GetNetMode();
    if (NetMode == ENetMode::NM_Client) {
        ServerInputReleased(InputID);
    }
}

float UCoreAbilitySystemComponent::GetInputPressTime(int32 InputID) const {
    auto FindTimePtr = InputTimeMap.Find(InputID);
    if (FindTimePtr) {
        auto DiffTime = FDateTime::Now() - *FindTimePtr;
        return DiffTime.GetTotalMilliseconds() * 0.001f;
    }
    return 0.f;
}

void UCoreAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) {
    Super::OnGiveAbility(AbilitySpec);

    //添加更新排序，todo，后面可以换成单个的插入排序，不用整体重新排序
    SortCoreAbilities.Empty();

    const auto& Abilities = GetActivatableAbilities();
    for (const FGameplayAbilitySpec& Spec : Abilities) {
        TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();

        for (UGameplayAbility* AbilityInstance : AbilityInstances) {
            auto ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
            if (ActiveAbility) {
                SortCoreAbilities.Add(ActiveAbility);
            }
        }
    }

    TFunction<bool(UCoreAbility*, UCoreAbility*)> CompareFunc = [](UCoreAbility* A, UCoreAbility* B) {
        return A->SortPriority > B->SortPriority;
    };
    USortUtils::SortArray(SortCoreAbilities, CompareFunc);
}

void UCoreAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) {
    Super::OnRemoveAbility(AbilitySpec);

    //移除更新排序
    TSubclassOf<UCoreAbility> AbilityClass = AbilitySpec.Ability->GetClass();
    bool HaveFound = false;
    for (int Index = SortCoreAbilities.Num() - 1; Index >= 0; --Index) {
        if (SortCoreAbilities[Index]->GetClass() == AbilityClass) {
            //同一类的ability是放在一起的，当for循环遍历完一类，后面就不用处理了
            HaveFound = true;
            SortCoreAbilities.RemoveAt(Index);
        }
        else if (HaveFound) {
            break;
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

bool UCoreAbilitySystemComponent::ServerInputPressed_Validate(int32 InputID) {
    return true;
}

void UCoreAbilitySystemComponent::ServerInputPressed_Implementation(int32 InputID) {
    InputPressedLocal(InputID);
}

bool UCoreAbilitySystemComponent::ServerInputReleased_Validate(int32 InputID) {
    return true;
}

void UCoreAbilitySystemComponent::ServerInputReleased_Implementation(int32 InputID) {
    InputReleasedLocal(InputID);
}

void UCoreAbilitySystemComponent::InputPressedLocal(int32 InputID) {
    auto FindTimePtr = InputTimeMap.Find(InputID);
    if (!FindTimePtr) {
        InputTimeMap.Add(InputID, FDateTime::Now());
    }

    for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items) {
        if (Spec.InputID == InputID) {
            TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();

            for (UGameplayAbility* AbilityInstance : AbilityInstances) {
                UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
                if (ActiveAbility && ActiveAbility->IsActive()) {
                    ActiveAbility->CallInputPressed(Spec.Handle);
                }
            }
        }
    }
}

void UCoreAbilitySystemComponent::InputReleasedLocal(int32 InputID) {
    if (InputTimeMap.Remove(InputID) > 0) {
        for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items) {
            if (Spec.InputID == InputID) {
                TArray<UGameplayAbility*> AbilityInstances = Spec.GetAbilityInstances();

                for (UGameplayAbility* AbilityInstance : AbilityInstances) {
                    UCoreAbility* ActiveAbility = Cast<UCoreAbility>(AbilityInstance);
                    if (ActiveAbility && ActiveAbility->IsActive()) {
                        ActiveAbility->CallInputReleased(Spec.Handle);
                    }
                }
            }
        }
    }
}

FGameplayTagContainer UCoreAbilitySystemComponent::GetTargetTagContainer() {
    FGameplayTagContainer TargetTagContainer;
    if (AActor* ThisAvatarActor = GetAvatarActor()) {
        auto FindEnemyComponent = Cast<UFindEnemyComponent>(ThisAvatarActor->GetComponentByClass(UFindEnemyComponent::StaticClass()));
        if (FindEnemyComponent) {
            auto TargetEnemy = FindEnemyComponent->JustGetEnemy();
            if (TargetEnemy) {
                auto TargetAbilitySystemComponent = TargetEnemy->GetAbilitySystemComponent();
                if (TargetAbilitySystemComponent) {
                    TargetAbilitySystemComponent->GetOwnedGameplayTags(TargetTagContainer);
                }
            }
        }
    }
    return TargetTagContainer;
}