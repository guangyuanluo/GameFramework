// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Slate.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Misc/Attribute.h"
#include "PropertyCustomizationHelpers.h"

class SSubClassSelectWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSubClassSelectWidget)
	: _MetaClass(UObject::StaticClass())
	, _AllowAbstract(false)
	{}

		/** The meta class that the selected class must be a child-of (required if PropertyEditor == null) */
		SLATE_ARGUMENT(UClass*, MetaClass)
		/** Whether or not abstract classes are allowed (optional) */
		SLATE_ARGUMENT(bool, AllowAbstract)
		/** Attribute used to get the currently selected class (required if PropertyEditor == null) */
		SLATE_ATTRIBUTE(const UClass*, SelectedClass)
		/** Delegate used to set the currently selected class (required if PropertyEditor == null) */
		SLATE_EVENT(FOnSetClass, OnSetClass)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	/** The meta class that the selected class must be a child-of */
	UClass* MetaClass;
	/** Whether or not abstract classes are allowed */
	bool bAllowAbstract;
	/** Attribute used to get the currently selected class (required if PropertyEditor == null) */
	TAttribute<const UClass*> SelectedClass;
	/** Delegate used to set the currently selected class (required if PropertyEditor == null) */
	FOnSetClass OnSetClass;

	TMap<FString, UClass*> ClassMap;
	TArray<TSharedPtr<FString>> ClassTreeItemSource;
	TMap<FString, TArray<FString>> CategoryClassMap;
	TSharedPtr<FString> SelectClassName;

	/** The tree view widget for our plugin categories tree */
	TSharedPtr<STreeView<TSharedPtr<FString>>> TreeView;
	/** Used when the property deals with Classes and will display a Class Picker. */
	TSharedPtr<class SComboButton> ComboButton;
	/** The Class Search Box, used for filtering the classes visible. */
	TSharedPtr<class SSearchBox> SearchBox;
	FString FilterText;

	/**
	 * Generates a class picker with a filter to show only classes allowed to be selected.
	 *
	 * @return The Class Picker widget.
	 */
	TSharedRef<SWidget> GenerateClassPicker();
	/**
	 * Gets the active display value as a string
	 */
	FText GetDisplayValueAsString() const;

	TSharedRef<ITableRow> MakeTableRowWidget(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void HandleGetChildrenForTree(TSharedPtr<FString> InItem, TArray<TSharedPtr<FString>>& OutChildren);
	void OnSelectionChanged(TSharedPtr<FString> Selection, ESelectInfo::Type SelectInfo);

	void OnFilterTextChanged(const FText& InFilterText);
};