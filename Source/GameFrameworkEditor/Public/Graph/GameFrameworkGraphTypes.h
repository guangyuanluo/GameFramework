// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/WeakObjectPtr.h"
#include "GameFrameworkGraphTypes.generated.h"

USTRUCT()
struct GAMEFRAMEWORKEDITOR_API FGameFrameworkGraphNodeClassData
{
	GENERATED_USTRUCT_BODY()

	FGameFrameworkGraphNodeClassData() {}
	FGameFrameworkGraphNodeClassData(UClass* InClass, const FString& InDeprecatedMessage);
	FGameFrameworkGraphNodeClassData(const FString& InAssetName, const FString& InGeneratedClassPackage, const FString& InClassName, UClass* InClass);

	FString ToString() const;
	FString GetClassName() const;
	FText GetCategory() const;
	FString GetDisplayName() const;
	UClass* GetClass(bool bSilent = false);
	bool IsAbstract() const;

	FORCEINLINE bool IsBlueprint() const { return AssetName.Len() > 0; }
	FORCEINLINE bool IsDeprecated() const { return DeprecatedMessage.Len() > 0; }
	FORCEINLINE FString GetDeprecatedMessage() const { return DeprecatedMessage; }
	FORCEINLINE FString GetPackageName() const { return GeneratedClassPackage; }

	/** set when child class masked this one out (e.g. always use game specific class instead of engine one) */
	uint32 bIsHidden : 1;

	/** set when class wants to hide parent class from selection (just one class up hierarchy) */
	uint32 bHideParent : 1;

private:

	/** pointer to uclass */
	TWeakObjectPtr<UClass> Class;

	/** path to class if it's not loaded yet */
	UPROPERTY()
	FString AssetName;
	
	UPROPERTY()
	FString GeneratedClassPackage;

	/** resolved name of class from asset data */
	UPROPERTY()
	FString ClassName;

	/** User-defined category for this class */
	UPROPERTY()
	FText Category;

	/** message for deprecated class */
	FString DeprecatedMessage;
};

struct GAMEFRAMEWORKEDITOR_API FGraphNodeClassNode
{
	FGameFrameworkGraphNodeClassData Data;
	FString ParentClassName;

	TSharedPtr<FGraphNodeClassNode> ParentNode;
	TArray<TSharedPtr<FGraphNodeClassNode> > SubNodes;

	void AddUniqueSubNode(TSharedPtr<FGraphNodeClassNode> SubNode);
};

struct GAMEFRAMEWORKEDITOR_API FGraphNodeClassHelper
{
	DECLARE_MULTICAST_DELEGATE(FOnPackageListUpdated);

	FGraphNodeClassHelper(UClass* InRootClass);
	~FGraphNodeClassHelper();

	void GatherClasses(const UClass* BaseClass, TArray<FGameFrameworkGraphNodeClassData>& AvailableClasses);
	static FString GetDeprecationMessage(const UClass* Class);

	void OnAssetAdded(const struct FAssetData& AssetData);
	void OnAssetRemoved(const struct FAssetData& AssetData);
	void InvalidateCache();
	void OnHotReload(EReloadCompleteReason Reason);

	static void AddUnknownClass(const FGameFrameworkGraphNodeClassData& ClassData);
	static bool IsClassKnown(const FGameFrameworkGraphNodeClassData& ClassData);
	static FOnPackageListUpdated OnPackageListUpdated;

	static int32 GetObservedBlueprintClassCount(UClass* BaseNativeClass);
	static void AddObservedBlueprintClasses(UClass* BaseNativeClass);
	void UpdateAvailableBlueprintClasses();

private:

	UClass* RootNodeClass;
	TSharedPtr<FGraphNodeClassNode> RootNode;
	static TArray<FName> UnknownPackages;
	static TMap<UClass*, int32> BlueprintClassCount;

	TSharedPtr<FGraphNodeClassNode> CreateClassDataNode(const struct FAssetData& AssetData);
	TSharedPtr<FGraphNodeClassNode> FindBaseClassNode(TSharedPtr<FGraphNodeClassNode> Node, const FString& ClassName);
	void FindAllSubClasses(TSharedPtr<FGraphNodeClassNode> Node, TArray<FGameFrameworkGraphNodeClassData>& AvailableClasses);

	UClass* FindAssetClass(const FString& GeneratedClassPackage, const FString& AssetName);
	void BuildClassGraph();
	void AddClassGraphChildren(TSharedPtr<FGraphNodeClassNode> Node, TArray<TSharedPtr<FGraphNodeClassNode> >& NodeList);

	bool IsHidingClass(UClass* Class);
	bool IsHidingParentClass(UClass* Class);
	bool IsPackageSaved(FName PackageName);
};