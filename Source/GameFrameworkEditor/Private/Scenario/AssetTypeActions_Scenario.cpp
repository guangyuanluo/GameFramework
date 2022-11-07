// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_Scenario.h"

#include "Modules/Scenario/Scenario.h"
#include "Framework/Application/SlateApplication.h"
#include "ScenarioEditor.h"
#include "GameFrameworkEditor.h"
#include "Modules/Scenario/ScenarioNode.h"

uint32 FAssetTypeActions_Scenario::GetCategories()
{
	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
	return GameFrameworkEditorModule.GetAssetCategoryBit();
}

void FAssetTypeActions_Scenario::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto Object : InObjects)
	{
		auto Scenario = Cast<UScenario>(Object);
		if (Scenario != nullptr)
		{
			bool bFoundExisting = false;
			if (!bFoundExisting)
			{
				UE_LOG(LogTemp, Display, TEXT("Open Scenario Editor"));
				TSharedRef<FScenarioEditor> NewEditor(new FScenarioEditor());
				NewEditor->InitEditor(Mode, EditWithinLevelEditor, Object);
			}
		}
	}
}

UClass* FAssetTypeActions_Scenario::GetSupportedClass() const
{
	return UScenario::StaticClass();
}