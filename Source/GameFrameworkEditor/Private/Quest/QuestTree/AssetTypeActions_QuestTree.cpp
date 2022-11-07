// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_QuestTree.h"

#include "Modules/Quest/QuestTree.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFrameworkEditor.h"
#include "QuestTreeEditor.h"

uint32 FAssetTypeActions_QuestTree::GetCategories()
{
	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
	return GameFrameworkEditorModule.GetQuestCategoryBit();
}

void FAssetTypeActions_QuestTree::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) {
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto Object : InObjects) {
		auto QuestTree = Cast<UQuestTree>(Object);
		if (QuestTree != nullptr) {
			bool bFoundExisting = false;
			if (!bFoundExisting) {
				UE_LOG(LogTemp, Display, TEXT("Open QuestTree Editor"));
				TSharedRef<FQuestTreeEditor> NewEditor(new FQuestTreeEditor());
				NewEditor->InitEditor(Mode, EditWithinLevelEditor, Object);
			}
		}
	}
}

UClass* FAssetTypeActions_QuestTree::GetSupportedClass() const
{
	return UQuestTree::StaticClass();
}