// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Graph/GameFrameworkGraphTypes.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "Misc/FeedbackContext.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectIterator.h"
#include "Misc/PackageName.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Blueprint.h"
#include "AssetRegistry/AssetData.h"
#include "Editor.h"
#include "Logging/MessageLog.h"
#include "AssetRegistry/ARFilter.h"
#if ENGINE_MAJOR_VERSION > 4
#include "AssetRegistry/AssetRegistryModule.h"
#else
#include "AssetRegistryModule.h"
#endif

#define LOCTEXT_NAMESPACE "GameFramework SClassViewer"

FGameFrameworkGraphNodeClassData::FGameFrameworkGraphNodeClassData(UClass* InClass, const FString& InDeprecatedMessage) :
	bIsHidden(0),
	bHideParent(0),
	Class(InClass),
	DeprecatedMessage(InDeprecatedMessage)
{
	Category = GetCategory();

	if (InClass)
	{
		ClassName = InClass->GetName();
	}
}

FGameFrameworkGraphNodeClassData::FGameFrameworkGraphNodeClassData(const FString& InAssetName, const FString& InGeneratedClassPackage, const FString& InClassName, UClass* InClass) :
	bIsHidden(0),
	bHideParent(0),
	Class(InClass),
	AssetName(InAssetName),
	GeneratedClassPackage(InGeneratedClassPackage),
	ClassName(InClassName)
{
	Category = GetCategory();
}

FString FGameFrameworkGraphNodeClassData::ToString() const
{
	FString ShortName = GetDisplayName();
	if (!ShortName.IsEmpty())
	{
		return ShortName;
	}

	UClass* MyClass = Class.Get();
	if (MyClass)
	{
		FString ClassDesc = MyClass->GetName();

		if (MyClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint))
		{
			return ClassDesc.LeftChop(2);
		}

		const int32 ShortNameIdx = ClassDesc.Find(TEXT("_"));
		if (ShortNameIdx != INDEX_NONE)
		{
			ClassDesc = ClassDesc.Mid(ShortNameIdx + 1);
		}

		return ClassDesc;
	}

	return AssetName;
}

FString FGameFrameworkGraphNodeClassData::GetClassName() const
{
	return Class.IsValid() ? Class->GetName() : ClassName;
}

FString FGameFrameworkGraphNodeClassData::GetDisplayName() const
{
	return Class.IsValid() ? Class->GetMetaData(TEXT("DisplayName")) : FString();
}

FText FGameFrameworkGraphNodeClassData::GetCategory() const
{
	return Class.IsValid() ? Class->GetMetaDataText(TEXT("Category"), TEXT("UObjectCategory"), Class->GetFullGroupName(false)) : Category;
}

bool FGameFrameworkGraphNodeClassData::IsAbstract() const
{
	return Class.IsValid() ? Class.Get()->HasAnyClassFlags(CLASS_Abstract) : false;
}

UClass* FGameFrameworkGraphNodeClassData::GetClass(bool bSilent)
{
	UClass* RetClass = Class.Get();
	if (RetClass == NULL && GeneratedClassPackage.Len())
	{
		GWarn->BeginSlowTask(LOCTEXT("GameFramework LoadPackage", "GameFramework Loading Package..."), true);

		UPackage* Package = LoadPackage(NULL, *GeneratedClassPackage, LOAD_NoRedirects);
		if (Package)
		{
			Package->FullyLoad();

			UObject* Object = FindObject<UObject>(Package, *AssetName);

			GWarn->EndSlowTask();

			UBlueprint* BlueprintOb = Cast<UBlueprint>(Object);
			RetClass = BlueprintOb ? *BlueprintOb->GeneratedClass :
				Object ? Object->GetClass() :
				NULL;

			Class = RetClass;
		}
		else
		{
			GWarn->EndSlowTask();

			if (!bSilent)
			{
				FMessageLog EditorErrors("EditorErrors");
				EditorErrors.Error(LOCTEXT("GameFramework PackageLoadFail", "GameFramework Package Load Failed"));
				EditorErrors.Info(FText::FromString(GeneratedClassPackage));
				EditorErrors.Notify(LOCTEXT("GameFramework PackageLoadFail", "GameFramework Package Load Failed"));
			}
		}
	}

	return RetClass;
}

//////////////////////////////////////////////////////////////////////////
TArray<FName> FGameGraphNodeClassHelper::UnknownPackages;
TMap<UClass*, int32> FGameGraphNodeClassHelper::BlueprintClassCount;
FGameGraphNodeClassHelper::FOnPackageListUpdated FGameGraphNodeClassHelper::OnPackageListUpdated;

FGameGraphNodeClassHelper::FGameGraphNodeClassHelper(UClass* InRootClass)
{
	RootNodeClass = InRootClass;

	// Register with the Asset Registry to be informed when it is done loading up files.
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	AssetRegistryModule.Get().OnFilesLoaded().AddRaw(this, &FGameGraphNodeClassHelper::InvalidateCache);
	AssetRegistryModule.Get().OnAssetAdded().AddRaw(this, &FGameGraphNodeClassHelper::OnAssetAdded);
	AssetRegistryModule.Get().OnAssetRemoved().AddRaw(this, &FGameGraphNodeClassHelper::OnAssetRemoved);

	// Register to have Populate called when doing a Hot Reload.
	FCoreUObjectDelegates::ReloadCompleteDelegate.AddRaw(this, &FGameGraphNodeClassHelper::OnHotReload);

	// Register to have Populate called when a Blueprint is compiled.
	GEditor->OnBlueprintCompiled().AddRaw(this, &FGameGraphNodeClassHelper::InvalidateCache);
	GEditor->OnClassPackageLoadedOrUnloaded().AddRaw(this, &FGameGraphNodeClassHelper::InvalidateCache);

	UpdateAvailableBlueprintClasses();
}

FGameGraphNodeClassHelper::~FGameGraphNodeClassHelper()
{
	// Unregister with the Asset Registry to be informed when it is done loading up files.
	if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetRegistry")))
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		AssetRegistryModule.Get().OnFilesLoaded().RemoveAll(this);
		AssetRegistryModule.Get().OnAssetAdded().RemoveAll(this);
		AssetRegistryModule.Get().OnAssetRemoved().RemoveAll(this);

		// Unregister to have Populate called when doing a Hot Reload.
		FCoreUObjectDelegates::ReloadCompleteDelegate.RemoveAll(this);

		// Unregister to have Populate called when a Blueprint is compiled.
		if (UObjectInitialized())
		{
			// GEditor can't have been destructed before we call this or we'll crash.
			GEditor->OnBlueprintCompiled().RemoveAll(this);
			GEditor->OnClassPackageLoadedOrUnloaded().RemoveAll(this);
		}
	}
}

void FGameGraphNodeClassNode::AddUniqueSubNode(TSharedPtr<FGameGraphNodeClassNode> SubNode)
{
	for (int32 Idx = 0; Idx < SubNodes.Num(); Idx++)
	{
		if (SubNode->Data.GetClassName() == SubNodes[Idx]->Data.GetClassName())
		{
			return;
		}
	}

	SubNodes.Add(SubNode);
}

void FGameGraphNodeClassHelper::GatherClasses(const UClass* BaseClass, TArray<FGameFrameworkGraphNodeClassData>& AvailableClasses)
{
	const FString BaseClassName = BaseClass->GetName();
	if (!RootNode.IsValid())
	{
		BuildClassGraph();
	}

	TSharedPtr<FGameGraphNodeClassNode> BaseNode = FindBaseClassNode(RootNode, BaseClassName);
	FindAllSubClasses(BaseNode, AvailableClasses);
}

FString FGameGraphNodeClassHelper::GetDeprecationMessage(const UClass* Class)
{
	static FName MetaDeprecated = TEXT("DeprecatedNode");
	static FName MetaDeprecatedMessage = TEXT("DeprecationMessage");
	FString DefDeprecatedMessage("Please remove it!");
	FString DeprecatedPrefix("DEPRECATED");
	FString DeprecatedMessage;

	if (Class && Class->HasAnyClassFlags(CLASS_Native) && Class->HasMetaData(MetaDeprecated))
	{
		DeprecatedMessage = DeprecatedPrefix + TEXT(": ");
		DeprecatedMessage += Class->HasMetaData(MetaDeprecatedMessage) ? Class->GetMetaData(MetaDeprecatedMessage) : DefDeprecatedMessage;
	}

	return DeprecatedMessage;
}

bool FGameGraphNodeClassHelper::IsClassKnown(const FGameFrameworkGraphNodeClassData& ClassData)
{
	return !ClassData.IsBlueprint() || !UnknownPackages.Contains(*ClassData.GetPackageName());
}

void FGameGraphNodeClassHelper::AddUnknownClass(const FGameFrameworkGraphNodeClassData& ClassData)
{
	if (ClassData.IsBlueprint())
	{
		UnknownPackages.AddUnique(*ClassData.GetPackageName());
	}
}

bool FGameGraphNodeClassHelper::IsHidingParentClass(UClass* Class)
{
	static FName MetaHideParent = TEXT("HideParentNode");
	return Class && Class->HasAnyClassFlags(CLASS_Native) && Class->HasMetaData(MetaHideParent);
}

bool FGameGraphNodeClassHelper::IsHidingClass(UClass* Class)
{
	static FName MetaHideInEditor = TEXT("HiddenNode");
	return Class && Class->HasAnyClassFlags(CLASS_Native) && Class->HasMetaData(MetaHideInEditor);
}

bool FGameGraphNodeClassHelper::IsPackageSaved(FName PackageName)
{
	const bool bFound = FPackageName::SearchForPackageOnDisk(PackageName.ToString());
	return bFound;
}

void FGameGraphNodeClassHelper::OnAssetAdded(const struct FAssetData& AssetData)
{
	TSharedPtr<FGameGraphNodeClassNode> Node = CreateClassDataNode(AssetData);

	TSharedPtr<FGameGraphNodeClassNode> ParentNode;
	if (Node.IsValid())
	{
		ParentNode = FindBaseClassNode(RootNode, Node->ParentClassName);

		if (!IsPackageSaved(AssetData.PackageName))
		{
			UnknownPackages.AddUnique(AssetData.PackageName);
		}
		else
		{
			const int32 PrevListCount = UnknownPackages.Num();
			UnknownPackages.RemoveSingleSwap(AssetData.PackageName);

			if (UnknownPackages.Num() != PrevListCount)
			{
				OnPackageListUpdated.Broadcast();
			}
		}
	}

	if (ParentNode.IsValid())
	{
		ParentNode->AddUniqueSubNode(Node);
		Node->ParentNode = ParentNode;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	if (!AssetRegistryModule.Get().IsLoadingAssets())
	{
		UpdateAvailableBlueprintClasses();
	}
}

void FGameGraphNodeClassHelper::OnAssetRemoved(const struct FAssetData& AssetData)
{
	FString AssetClassName;
	if (AssetData.GetTagValue(FBlueprintTags::GeneratedClassPath, AssetClassName))
	{
		ConstructorHelpers::StripObjectClass(AssetClassName);
		AssetClassName = FPackageName::ObjectPathToObjectName(AssetClassName);

		TSharedPtr<FGameGraphNodeClassNode> Node = FindBaseClassNode(RootNode, AssetClassName);
		if (Node.IsValid() && Node->ParentNode.IsValid())
		{
			Node->ParentNode->SubNodes.RemoveSingleSwap(Node);
		}
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	if (!AssetRegistryModule.Get().IsLoadingAssets())
	{
		UpdateAvailableBlueprintClasses();
	}
}

void FGameGraphNodeClassHelper::InvalidateCache()
{
	RootNode.Reset();

	UpdateAvailableBlueprintClasses();
}

void FGameGraphNodeClassHelper::OnHotReload(EReloadCompleteReason Reason)
{
	InvalidateCache();
}

TSharedPtr<FGameGraphNodeClassNode> FGameGraphNodeClassHelper::CreateClassDataNode(const struct FAssetData& AssetData)
{
	TSharedPtr<FGameGraphNodeClassNode> Node;

	FString AssetClassName;
	FString AssetParentClassName;
	if (AssetData.GetTagValue(FBlueprintTags::GeneratedClassPath, AssetClassName) && AssetData.GetTagValue(FBlueprintTags::ParentClassPath, AssetParentClassName))
	{
		UObject* Outer1(NULL);
		ResolveName(Outer1, AssetClassName, false, false);

		UObject* Outer2(NULL);
		ResolveName(Outer2, AssetParentClassName, false, false);

		Node = MakeShareable(new FGameGraphNodeClassNode);
		Node->ParentClassName = AssetParentClassName;

		UObject* AssetOb = AssetData.IsAssetLoaded() ? AssetData.GetAsset() : NULL;
		UBlueprint* AssetBP = Cast<UBlueprint>(AssetOb);
		UClass* AssetClass = AssetBP ? *AssetBP->GeneratedClass : AssetOb ? AssetOb->GetClass() : NULL;

		FGameFrameworkGraphNodeClassData NewData(AssetData.AssetName.ToString(), AssetData.PackageName.ToString(), AssetClassName, AssetClass);
		Node->Data = NewData;
	}

	return Node;
}

TSharedPtr<FGameGraphNodeClassNode> FGameGraphNodeClassHelper::FindBaseClassNode(TSharedPtr<FGameGraphNodeClassNode> Node, const FString& ClassName)
{
	TSharedPtr<FGameGraphNodeClassNode> RetNode;
	if (Node.IsValid())
	{
		if (Node->Data.GetClassName() == ClassName)
		{
			return Node;
		}

		for (int32 i = 0; i < Node->SubNodes.Num(); i++)
		{
			RetNode = FindBaseClassNode(Node->SubNodes[i], ClassName);
			if (RetNode.IsValid())
			{
				break;
			}
		}
	}

	return RetNode;
}

void FGameGraphNodeClassHelper::FindAllSubClasses(TSharedPtr<FGameGraphNodeClassNode> Node, TArray<FGameFrameworkGraphNodeClassData>& AvailableClasses)
{
	if (Node.IsValid())
	{
		if (!Node->Data.IsAbstract() && !Node->Data.IsDeprecated() && !Node->Data.bIsHidden)
		{
			AvailableClasses.Add(Node->Data);
		}

		for (int32 i = 0; i < Node->SubNodes.Num(); i++)
		{
			FindAllSubClasses(Node->SubNodes[i], AvailableClasses);
		}
	}
}

UClass* FGameGraphNodeClassHelper::FindAssetClass(const FString& GeneratedClassPackage, const FString& AssetName)
{
	UPackage* Package = FindPackage(NULL, *GeneratedClassPackage);
	if (Package)
	{
		UObject* Object = FindObject<UObject>(Package, *AssetName);
		if (Object)
		{
			UBlueprint* BlueprintOb = Cast<UBlueprint>(Object);
			return BlueprintOb ? *BlueprintOb->GeneratedClass : Object->GetClass();
		}
	}

	return NULL;
}

void FGameGraphNodeClassHelper::BuildClassGraph()
{
	TArray<TSharedPtr<FGameGraphNodeClassNode> > NodeList;
	TArray<UClass*> HideParentList;
	RootNode.Reset();

	// gather all native classes
	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* TestClass = *It;
		if (TestClass->HasAnyClassFlags(CLASS_Native) && TestClass->IsChildOf(RootNodeClass))
		{
			TSharedPtr<FGameGraphNodeClassNode> NewNode = MakeShareable(new FGameGraphNodeClassNode);
			NewNode->ParentClassName = TestClass->GetSuperClass()->GetName();

			FString DeprecatedMessage = GetDeprecationMessage(TestClass);
			FGameFrameworkGraphNodeClassData NewData(TestClass, DeprecatedMessage);

			NewData.bHideParent = IsHidingParentClass(TestClass);
			if (NewData.bHideParent)
			{
				HideParentList.Add(TestClass->GetSuperClass());
			}

			NewData.bIsHidden = IsHidingClass(TestClass);

			NewNode->Data = NewData;

			if (TestClass == RootNodeClass)
			{
				RootNode = NewNode;
			}

			NodeList.Add(NewNode);
		}
	}

	// find all hidden parent classes
	for (int32 i = 0; i < NodeList.Num(); i++)
	{
		TSharedPtr<FGameGraphNodeClassNode> TestNode = NodeList[i];
		if (HideParentList.Contains(TestNode->Data.GetClass()))
		{
			TestNode->Data.bIsHidden = true;
		}
	}

	// gather all blueprints
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> BlueprintList;

	FARFilter Filter;
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	AssetRegistryModule.Get().GetAssets(Filter, BlueprintList);

	for (int32 i = 0; i < BlueprintList.Num(); i++)
	{
		TSharedPtr<FGameGraphNodeClassNode> NewNode = CreateClassDataNode(BlueprintList[i]);
		NodeList.Add(NewNode);
	}

	// build class tree
	AddClassGraphChildren(RootNode, NodeList);
}

void FGameGraphNodeClassHelper::AddClassGraphChildren(TSharedPtr<FGameGraphNodeClassNode> Node, TArray<TSharedPtr<FGameGraphNodeClassNode> >& NodeList)
{
	if (!Node.IsValid())
	{
		return;
	}

	const FString NodeClassName = Node->Data.GetClassName();
	for (int32 i = NodeList.Num() - 1; i >= 0; i--)
	{
		if (NodeList[i]->ParentClassName == NodeClassName)
		{
			TSharedPtr<FGameGraphNodeClassNode> MatchingNode = NodeList[i];
			NodeList.RemoveAt(i);

			MatchingNode->ParentNode = Node;
			Node->SubNodes.Add(MatchingNode);

			AddClassGraphChildren(MatchingNode, NodeList);
		}
	}
}

int32 FGameGraphNodeClassHelper::GetObservedBlueprintClassCount(UClass* BaseNativeClass)
{
	return BlueprintClassCount.FindRef(BaseNativeClass);
}

void FGameGraphNodeClassHelper::AddObservedBlueprintClasses(UClass* BaseNativeClass)
{
	BlueprintClassCount.Add(BaseNativeClass, 0);
}

void FGameGraphNodeClassHelper::UpdateAvailableBlueprintClasses()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetRegistry")))
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		const bool bSearchSubClasses = true;

		TArray<FTopLevelAssetPath> ClassNames;
		TSet<FTopLevelAssetPath> DerivedClassNames;

		for (TMap<UClass*, int32>::TIterator It(BlueprintClassCount); It; ++It)
		{
			ClassNames.Reset();
			ClassNames.Add(It.Key()->GetClassPathName());

			DerivedClassNames.Empty(DerivedClassNames.Num());
			AssetRegistryModule.Get().GetDerivedClassNames(ClassNames, TSet<FTopLevelAssetPath>(), DerivedClassNames);

			int32& Count = It.Value();
			Count = DerivedClassNames.Num();
		}
	}
}

#undef LOCTEXT_NAMESPACE
