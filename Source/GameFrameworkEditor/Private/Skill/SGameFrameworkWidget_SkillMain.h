// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slate.h"
#include "Widgets/SCompoundWidget.h"
#include "EditorCommon.h"

/**
 * 
 */
class SGameFrameworkWidget_SkillMain : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameFrameworkWidget_SkillMain) {
	}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<FUICommandList> InCommandList);
	virtual ~SGameFrameworkWidget_SkillMain() override;

	/**-----------------界面相关---------------------**/
	int mTabIndex;
	TSharedPtr<FUICommandList> CommandList;

	TSharedRef<SWidget> ConstructPage();
	int32 GetTabIndex() const;
	TSharedRef<SWidget> ConstructPageButton(const FString& PageName, FOnClicked ClickCallback);
	FReply SkillTabClicked();
	FReply EffectTabClicked();
	FReply SkillGroupTabClicked();

	TSharedRef<SWidget> ConstructSkillPage();
	TSharedRef<SWidget> ConstructEffectPage();
	TSharedRef<SWidget> ConstructSkillGroupPage();
};