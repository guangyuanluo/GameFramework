// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GameFrameworkEditor.h"
#include "GameFrameworkEditorStyle.h"
#include "GameFrameworkEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "SGameFrameworkMainWidget.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Scenario/AssetTypeActions_Scenario.h"
#include "Modules/Scenario/ScenarioNode.h"
#include "GraphPanelNodeFactoryCommon.h"
#include "Scenario/ScenarioEditor.h"
#include "PropertyEditorModule.h"
#include "BaseConditionWidgetFactories.h"
#include "EditorModeRegistry.h"
#include "IPlacementModeModule.h"
#include "EdGraphUtilities.h"

#include "SUnitPlacementPalette.h"

#include "GameFrameworkEditorWidgetFactory_Exp.h"
#include "GameFrameworkEditorWidgetFactory_Money.h"
#include "GameFrameworkEditorWidgetFactory_Unit.h"
#include "GameFrameworkEditorWidgetFactory_Item.h"
#include "GameFrameworkEditorWidgetFactory_UnitGroup.h"
#include "GameFrameworkEditorWidgetFactory_Skill.h"
#include "GameFrameworkEditorWidgetFactory_Store.h"

#include "Character/CoreCharacter.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionList.h"

#include "Modules/Skill/SkillInfo.h"
#include "Modules/Money/MoneyTypeNumPair.h"
#include "Customization/MoneyTypeNumPairCustomization.h"
#include "Modules/Item/ItemIDNumPair.h"
#include "Customization/ItemIDNumPairCustomization.h"
#include "Modules/Exp/ExpTypeNumPair.h"
#include "Customization/ExpTypeNumPairCustomization.h"
#include "CoreConditionCustomization.h"
#include "CoreConditionListCustomization.h"
#include "Customization/ScenarioNodeCustomization.h"
#include "AssetTypeActions_QuestDetail.h"
#include "AssetTypeActions_QuestTree.h"
#include "Customization/QuestCustomization.h"
#include "Modules/Quest/Quest.h"
#include "Customization/QuestDetailNodeItemCustomization.h"
#include "Modules/Quest/QuestDetailNodeItem.h"
#include "Customization/SkillInfoCustomization.h"
#include "Customization/EffectInfoCustomization.h"

static const FName GameFrameworkEditorTabName("GameFrameworkEditor");

#define LOCTEXT_NAMESPACE "FGameFrameworkEditorModule"

void FGameFrameworkEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGameFrameworkEditorStyle::Initialize();
	FGameFrameworkEditorStyle::ReloadTextures();

	FGameFrameworkEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGameFrameworkEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGameFrameworkEditorModule::PluginButtonClicked),
		FCanExecuteAction());
		
    RegistMenuExtender();
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GameFrameworkEditorTabName, FOnSpawnTab::CreateRaw(this, &FGameFrameworkEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGameFrameworkEditorTabTitle", "GameFrameworkEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	// ??????????????????
	RegistAssetType();

    //????????????????????????
    BaseConditionWidgetFactories::Init();

    //??????????????????
    RegistCustomPlacement(FText::FromString(TEXT("??????")), TEXT("Unit"), []() -> TSharedRef<SWidget> { return SNew(SUnitPlacementPalette); });

	//??????????????????
    RegistAllEditorWidgetFactories();

    //?????????????????????
    RegistCustomizationDetail();
}

void FGameFrameworkEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGameFrameworkEditorStyle::Shutdown();

	FGameFrameworkEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GameFrameworkEditorTabName);

	// Unregister the scenario item data asset type actions
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto& AssetTypeAction : ItemDataAssetTypeActions)
		{
			if (AssetTypeAction.IsValid())
			{
				AssetToolsModule.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
			}
		}
	}
	ItemDataAssetTypeActions.Empty();
	mRegistEditorWidgetFactories.Empty();

    BaseConditionWidgetFactories::Uninit();

	if (GraphPanelNodeFactoryCommon.IsValid()) {
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactoryCommon);
		GraphPanelNodeFactoryCommon.Reset();
	}

    UnregistCustomizationDetail();
}

void FGameFrameworkEditorModule::RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory> EditorWidgetFactory) {
	mRegistEditorWidgetFactories.Add(EditorWidgetFactory);
}

void FGameFrameworkEditorModule::RegistGameFrameworkEditorWidgetTool(TSharedPtr<GameFrameworkEditorWidgetTool> EditorWidgetTool) {
	mEditorWidgetTool = EditorWidgetTool;
}

const TArray<TSharedPtr<GameFrameworkEditorWidgetFactory>>& FGameFrameworkEditorModule::GetRegistEditorWidgetFactories() const {
	return mRegistEditorWidgetFactories;
}

TSharedPtr<GameFrameworkEditorWidgetTool> FGameFrameworkEditorModule::GetEditorWidgetTool() {
	if (!mEditorWidgetTool.IsValid()) {
		mEditorWidgetTool = TSharedPtr<GameFrameworkEditorWidgetTool>(new GameFrameworkEditorWidgetTool());
	}
	return mEditorWidgetTool;
}

void FGameFrameworkEditorModule::RegistCustomPlacement(const FText& PlacementTypeName, const FString& Category, TFunction<TSharedRef<SWidget>()> CustomGenerator) {
    IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
    FName CategoryName = *Category;
    FPlacementCategoryInfo CategoryInfo(PlacementTypeName, CategoryName, Category, 35);
    CategoryInfo.CustomGenerator = CustomGenerator;
    PlacementModeModule.RegisterPlacementCategory(CategoryInfo);
}

TSharedRef<SDockTab> FGameFrameworkEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SAssignNew(mWidget, SGameFrameworkMainWidget, PluginCommands)
		];
}

void FGameFrameworkEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GameFrameworkEditorTabName);
}

uint32 FGameFrameworkEditorModule::GetAssetCategoryBit()
{
	return AssetCategoryBit;
}

uint32 FGameFrameworkEditorModule::GetQuestCategoryBit() {
	return QuestCategoryBit;
}

void FGameFrameworkEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FGameFrameworkEditorCommands::Get().OpenPluginWindow);
}

void FGameFrameworkEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FGameFrameworkEditorCommands::Get().OpenPluginWindow);
}

void FGameFrameworkEditorModule::RegistMenuExtender() {
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    {
        TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
        MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FGameFrameworkEditorModule::AddMenuExtension));

        LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
    }

    {
        TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
        ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FGameFrameworkEditorModule::AddToolbarExtension));

        LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
    }
}

void FGameFrameworkEditorModule::RegistAssetType() {
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("GameFramework")), FText::FromString(TEXT("GameFramework")));
	QuestCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Quest")), FText::FromString(TEXT("??????")));

    IAssetTools& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    TSharedPtr<FAssetTypeActions_Scenario> ScenarioAssetTypeAction = MakeShareable(new FAssetTypeActions_Scenario);
    ItemDataAssetTypeActions.Add(ScenarioAssetTypeAction);
	AssetToolsModule.RegisterAssetTypeActions(ScenarioAssetTypeAction.ToSharedRef());

	TSharedPtr<FAssetTypeActions_QuestDetail> QuestDetailAssetTypeAction = MakeShareable(new FAssetTypeActions_QuestDetail);
	ItemDataAssetTypeActions.Add(QuestDetailAssetTypeAction);
	AssetToolsModule.RegisterAssetTypeActions(QuestDetailAssetTypeAction.ToSharedRef());

	TSharedPtr<FAssetTypeActions_QuestTree> AssetTypeActions_QuestTree = MakeShareable(new FAssetTypeActions_QuestTree);
	ItemDataAssetTypeActions.Add(AssetTypeActions_QuestTree);
	AssetToolsModule.RegisterAssetTypeActions(AssetTypeActions_QuestTree.ToSharedRef());

    GraphPanelNodeFactoryCommon = MakeShareable(new FGraphPanelNodeFactoryCommon());
    FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactoryCommon);
}

void FGameFrameworkEditorModule::RegistAllEditorWidgetFactories() {
    RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory>(new GameFrameworkEditorWidgetFactory_Exp()));
    RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory>(new GameFrameworkEditorWidgetFactory_Money()));
    RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory>(new GameFrameworkEditorWidgetFactory_Unit()));
    RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory>(new GameFrameworkEditorWidgetFactory_Item()));
    RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory>(new GameFrameworkEditorWidgetFactory_UnitGroup()));
    RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory>(new GameFrameworkEditorWidgetFactory_Skill()));
	RegistEditorWidgetFactory(TSharedPtr<GameFrameworkEditorWidgetFactory>(new GameFrameworkEditorWidgetFactory_Store()));
}

void FGameFrameworkEditorModule::RegistCustomizationDetail() {
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FMoneyTypeNumPair::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMoneyTypeNumPairCustomization::MakeInstance));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FItemIDNumPair::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FItemIDNumPairCustomization::MakeInstance));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FExpTypeNumPair::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FExpTypeNumPairCustomization::MakeInstance));

	PropertyModule.RegisterCustomClassLayout(
		UQuest::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FQuestCustomization::MakeInstance));

	PropertyModule.RegisterCustomClassLayout(
		UQuestDetailNodeItem::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FQuestDetailNodeItemCustomization::MakeInstance));

	PropertyModule.RegisterCustomClassLayout(
		UCoreCondition::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FCoreConditionCustomization::MakeInstance));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FCoreConditionList::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FCoreConditionListCustomization::MakeInstance));

	PropertyModule.RegisterCustomClassLayout(
		UScenarioNode::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FScenarioNodeCustomization::MakeInstance));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FSkillInfo::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSkillInfoCustomization::MakeInstance));

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FEffectInfo::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FEffectInfoCustomization::MakeInstance));

    PropertyModule.NotifyCustomizationModuleChanged();
}

void FGameFrameworkEditorModule::UnregistCustomizationDetail() {
    if (FModuleManager::Get().IsModuleLoaded("PropertyEditor")) {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FMoneyTypeNumPair::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FItemIDNumPair::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FExpTypeNumPair::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UQuest::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UQuestDetailNodeItem::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UCoreCondition::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UScenarioNode::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomClassLayout(UScenarioNode::StaticClass()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSkillInfo::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FEffectInfo::StaticStruct()->GetFName());

        PropertyModule.NotifyCustomizationModuleChanged();
    }
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameFrameworkEditorModule, GameFrameworkEditor)