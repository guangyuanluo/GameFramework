// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_QuestDetail.h"

#include "Modules/Quest/QuestDetail.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFrameworkEditor.h"
#include "QuestDetailEditor.h"

uint32 FAssetTypeActions_QuestDetail::GetCategories()
{
	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
	return GameFrameworkEditorModule.GetQuestCategoryBit();
}

void FAssetTypeActions_QuestDetail::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) {
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto Object : InObjects) {
		auto QuestDetail = Cast<UQuestDetail>(Object);
		if (QuestDetail != nullptr) {
			bool bFoundExisting = false;
			if (!bFoundExisting) {
				UE_LOG(LogTemp, Display, TEXT("Open QuestDetail Editor"));
				TSharedRef<FQuestDetailEditor> NewEditor(new FQuestDetailEditor());
				NewEditor->InitEditor(Mode, EditWithinLevelEditor, Object);
			}
		}
	}
}

UClass* FAssetTypeActions_QuestDetail::GetSupportedClass() const
{
	return UQuestDetail::StaticClass();
}