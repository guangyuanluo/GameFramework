// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Developer/AssetTools/Public/AssetTypeCategories.h"
#include "Editor/UnrealEd/Public/Toolkits/IToolkit.h"
#include "Editor/UnrealEd/Public/Toolkits/IToolkitHost.h"

class SGameFrameworkMainWidget;
class FToolBarBuilder;
class FMenuBuilder;
class GameFrameworkEditorWidgetFactory;
class GameFrameworkEditorWidgetTool;
class AActor;

class GAMEFRAMEWORKEDITOR_API FGameFrameworkEditorModule : public IModuleInterface
{
public:
	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline FGameFrameworkEditorModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FGameFrameworkEditorModule >("GameFrameworkEditor");
	}
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory> EditorWidgetFactory);
	void RegistGameFrameworkEditorWidgetTool(TSharedPtr<GameFrameworkEditorWidgetTool> EditorWidgetTool);
	const TArray<TSharedPtr<GameFrameworkEditorWidgetFactory>>& GetRegistEditorWidgetFactories() const;
	TSharedPtr<GameFrameworkEditorWidgetTool> GetEditorWidgetTool();

    void RegistCustomPlacement(const FText& PlacementTypeName, const FString& Category, TFunction<TSharedRef<SWidget>()> CustomGenerator);
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	virtual uint32 GetAssetCategoryBit();
	virtual uint32 GetQuestCategoryBit();
private:
	/** Asset type actions */
	TArray<TSharedPtr<class FAssetTypeActions_Base>> ItemDataAssetTypeActions;
	EAssetTypeCategories::Type AssetCategoryBit;
	EAssetTypeCategories::Type QuestCategoryBit;
	TSharedPtr<struct FGraphPanelNodeFactory> GraphPanelNodeFactoryCommon;

private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<SGameFrameworkMainWidget> mWidget;

	TSharedPtr<class FUICommandList> PluginCommands;
	
	TSharedPtr<GameFrameworkEditorWidgetTool> mEditorWidgetTool;
	TArray<TSharedPtr<GameFrameworkEditorWidgetFactory>> mRegistEditorWidgetFactories;

    void RegistMenuExtender();
    void RegistAssetType();
    void RegistAllEditorWidgetFactories();
    void RegistCustomizationDetail();

    void UnregistCustomizationDetail();
};