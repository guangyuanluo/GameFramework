// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreAbilityCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "PropertyRestriction.h"

#include "Modules/Skill/CoreAbility.h"

void FCoreAbilityCustomization::CustomizeDetails(IDetailLayoutBuilder& LayoutBuilder) {
    auto LimitActiveCounterProperty = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, LimitActiveCounter));

    int LimitActiveCounter;
    LimitActiveCounterProperty->GetValue(LimitActiveCounter);
    bool ShowCounterEdit = LimitActiveCounter != 0;

    auto EffectContainerMapProperty = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, EffectContainerMap));
    void* DataPtr;
    EffectContainerMapProperty->GetValueData(DataPtr);
    TMap<FGameplayTag, FCoreGameplayEffectContainer>* MapPtr = (TMap<FGameplayTag, FCoreGameplayEffectContainer>*)DataPtr;
    for (auto Iter = MapPtr->CreateIterator(); Iter; ++Iter) {
        FCoreGameplayEffectContainer& Value = Iter->Value;
        Value.ShowCounterEdit = ShowCounterEdit;
    }

    LimitActiveCounterProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([&LayoutBuilder]() {
        LayoutBuilder.ForceRefreshDetails();
    }));

    const FString& GroupRelationPropertyName = GET_MEMBER_NAME_CHECKED(FCoreAbilityConditionGroupInfo, Relation).ToString();
    const FString& GroupConditionPropertyName = GET_MEMBER_NAME_CHECKED(FCoreAbilityConditionGroupInfo, ConditionConfigs).ToString();
    const FString& ConditionRelationPropertyName = GET_MEMBER_NAME_CHECKED(FCoreAbilityConditionConfig, Relation).ToString();

    auto GroupConditionConfigsProperty = LayoutBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCoreAbility, GroupConditionConfigs));

    GroupConditionConfigsProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([&LayoutBuilder]() {
        LayoutBuilder.ForceRefreshDetails();
    }));

    uint32 GroupChildrenNum;
    GroupConditionConfigsProperty->GetNumChildren(GroupChildrenNum);
    if (GroupChildrenNum > 0) {
        for (uint32 GroupIndex = 0; GroupIndex < GroupChildrenNum; ++GroupIndex) {
            auto GroupItemProperties = GroupConditionConfigsProperty->GetChildHandle(GroupIndex);
            uint32 GroupPropertyNum;
            GroupItemProperties->GetNumChildren(GroupPropertyNum);
            if (GroupIndex == 0) {
                //隐藏第一个组的relation
                for (uint32 GroupPropertyIndex = 0; GroupPropertyIndex < GroupPropertyNum; ++GroupPropertyIndex) {
                    auto GroupItemProperty = GroupItemProperties->GetChildHandle(GroupPropertyIndex);
                    if (GroupItemProperty->GetProperty()->GetName() == GroupRelationPropertyName) {
                        LayoutBuilder.HideProperty(GroupItemProperty);
                        break;
                    }
                }
            }
            //组内每个条件数组的第一个元素也要隐藏Relation编辑
            for (uint32 GroupPropertyIndex = 0; GroupPropertyIndex < GroupPropertyNum; ++GroupPropertyIndex) {
                auto GroupItemProperty = GroupItemProperties->GetChildHandle(GroupPropertyIndex);
                if (GroupItemProperty->GetProperty()->GetName() == GroupConditionPropertyName) {
                    GroupItemProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([&LayoutBuilder]() {
                        LayoutBuilder.ForceRefreshDetails();
                    }));

                    //找到Group的Condition属性
                    uint32 NumGroupConditions;
                    GroupItemProperty->GetNumChildren(NumGroupConditions);
                    if (NumGroupConditions > 0) {
                        auto ConditionItemProperty = GroupItemProperty->GetChildHandle(0);
                        uint32 NumCondtionItemProperty;
                        ConditionItemProperty->GetNumChildren(NumCondtionItemProperty);
                        for (uint32 ConditionItemIndex = 0; ConditionItemIndex < NumCondtionItemProperty; ++ConditionItemIndex) {
                            auto ConditionProperty = ConditionItemProperty->GetChildHandle(ConditionItemIndex);
                            if (ConditionProperty->GetProperty()->GetName() == ConditionRelationPropertyName) {
                                LayoutBuilder.HideProperty(ConditionProperty);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

TSharedRef<IDetailCustomization> FCoreAbilityCustomization::MakeInstance() {
	return MakeShareable(new FCoreAbilityCustomization());
}