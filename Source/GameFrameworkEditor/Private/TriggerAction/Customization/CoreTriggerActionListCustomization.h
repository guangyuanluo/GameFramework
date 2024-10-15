// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IPropertyTypeCustomization.h"

class FCoreTriggerActionListCustomization : public IPropertyTypeCustomization {
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
    TSharedPtr<IPropertyHandle> ActionsProperty;

    void OnAssetChange();
};