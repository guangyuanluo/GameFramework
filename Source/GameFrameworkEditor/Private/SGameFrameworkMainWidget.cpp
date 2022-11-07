// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameFrameworkMainWidget.h"
#include "SlateOptMacros.h"
#include "GameFrameworkEditorCommands.h"
#include "GameFrameworkEditor.h"
#include "Runtime/Slate/Public/Widgets/Layout/SUniformGridPanel.h"
#include "GameFrameworkEditorWidgetFactory.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"

#include "Editor/Table/DataTableSeedConfigTableHelper.h"
#include "Editor/Table/DataTableSeedConfigTableRow.h"
#include "Developer/AssetTools/Public/AssetToolsModule.h"
#include "Editor/UnrealEd/Classes/Factories/DataTableFactory.h"
#include "GameFrameworkEditorWidgetTool.h"
#include "GameFrameworkEditor.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameFrameworkMainWidget::Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList)
{
	CommandList = InCommandList;

	FGameFrameworkEditorModule& GameFrameworkEditorModule = FModuleManager::LoadModuleChecked<FGameFrameworkEditorModule>("GameFrameworkEditor").Get();
	const auto& RregistEditorWidgetFactories = GameFrameworkEditorModule.GetRegistEditorWidgetFactories();

	CheckBaseEditorTableNoExistAndCreate();
	for (int Index = 0; Index < RregistEditorWidgetFactories.Num(); ++Index) {
		auto RegistEditorWidgetFactory = RregistEditorWidgetFactories[Index];
        RegistEditorWidgetFactory->CheckEditorTableNoExistAndCreate();
	}

	TSharedPtr<SUniformGridPanel> mainPanel;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(0.1)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()			
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("导出")))
				.ToolTipText(FText::FromString(TEXT("导出编辑后的所有代码部分")))
				.OnClicked_Lambda([RregistEditorWidgetFactories, this]() {
					//各个编辑器模块窗口导出
					for (int Index = 0; Index < RregistEditorWidgetFactories.Num(); ++Index) {
						auto RegistEditorWidgetFactory = RregistEditorWidgetFactories[Index];
						RegistEditorWidgetFactory->Export();
					}

					FGameFrameworkEditorModule::Get().GetEditorWidgetTool()->ShowNotification(FText::FromString(TEXT("导出成功")), 5.0);

					return(FReply::Handled());
				})
			]
		]
		+ SVerticalBox::Slot()
		[
			// Populate the widget
			SAssignNew(mainPanel, SUniformGridPanel)
			.SlotPadding(3)
			.MinDesiredSlotWidth(20.0f)
			.MinDesiredSlotHeight(20.0f)
		]
	];

	for (int Index = 0; Index < RregistEditorWidgetFactories.Num(); ++Index) {
		auto RegistEditorWidgetFactory = RregistEditorWidgetFactories[Index];
		int Column = Index % 2;
		int Row = Index / 2;
		mainPanel->AddSlot(Column, Row)
			[
				SNew(SButton)
				.Text(RegistEditorWidgetFactory->GetDisplayName())
				.ToolTipText(RegistEditorWidgetFactory->GetDisplayToolTip())
				.OnClicked_Lambda([RegistEditorWidgetFactory, this]() {
                    if (RegistEditorWidgetFactory->CheckOpenCondition()) {
                        const FVector2D BrowserWindowSize(1280, 720);
                        TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
                        FSlateApplication::Get().AddModalWindow(
                            SNew(SWindow)
                            .Title(RegistEditorWidgetFactory->GetWindowName())
                            .HasCloseButton(true)
                            .ClientSize(BrowserWindowSize)
                            [
                                RegistEditorWidgetFactory->ConstructPage(CommandList)
                            ], RootWindow);
                    }

					return(FReply::Handled());
				})
			];
	}
}

void SGameFrameworkMainWidget::CheckBaseEditorTableNoExistAndCreate() {
	auto seedDataTable = DataTableSeedConfigTableHelper::GetDataTable();
	if (seedDataTable == nullptr) {
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		UDataTableFactory* seedTableFactory = NewObject<UDataTableFactory>();
		seedTableFactory->Struct = FDataTableSeedConfigTableRow::StaticStruct();

		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*DataTableSeedConfigTableHelper::GetDataTablePackagePath())) {
			FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*DataTableSeedConfigTableHelper::GetDataTablePackagePath());
		}
		AssetToolsModule.Get().CreateAsset(DataTableSeedConfigTableHelper::GetDataTableAssetName(), DataTableSeedConfigTableHelper::GetDataTablePackagePath(), UDataTable::StaticClass(), seedTableFactory, FName("ContentBrowserNewAsset"));
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION