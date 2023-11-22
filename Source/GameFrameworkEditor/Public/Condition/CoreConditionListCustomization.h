// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PropertyEditor/Public/IPropertyTypeCustomization.h"

class GAMEFRAMEWORKEDITOR_API FCoreConditionListCustomization : public IPropertyTypeCustomization {
public:
    /**
     * It is just a convenient helpers which will be used
     * to register our customization. When the propertyEditor module
     * find our FMyStruct property, it will use this static method
     * to instanciate our customization object.
     */
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    // BEGIN IPropertyTypeCustomization interface
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
        class FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
        class IDetailChildrenBuilder& StructBuilder,
        IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
    // END IPropertyTypeCustomization interface

private:
    TSharedPtr<IPropertyHandle> ConditionsProperty;

    void OnAssetChange();
};