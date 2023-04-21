// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_AnimSearchTree.h"

#include "Modules/AnimSearchTree/AnimSearchTree.h"
#include "Framework/Application/SlateApplication.h"
#include "CustomizableSearchTree/CustomizableSearchTreeEditor.h"
#include "GameFrameworkEditor.h"

uint32 FAssetTypeActions_AnimSearchTree::GetCategories()
{
	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
	return GameFrameworkEditorModule.GetAssetCategoryBit();
}

void FAssetTypeActions_AnimSearchTree::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto Object : InObjects)
	{
		auto CustomizableSearchTree = Cast<UCustomizableSearchTree>(Object);
		if (CustomizableSearchTree != nullptr)
		{
			bool bFoundExisting = false;
			if (!bFoundExisting)
			{
				UE_LOG(LogTemp, Display, TEXT("Open CustomizableSearchTree Editor"));
				TSharedRef<FCustomizableSearchTreeEditor> NewEditor(new FCustomizableSearchTreeEditor());
				NewEditor->InitEditor(Mode, EditWithinLevelEditor, Object);
			}
		}
	}
}

UClass* FAssetTypeActions_AnimSearchTree::GetSupportedClass() const
{
	return UAnimSearchTree::StaticClass();
}