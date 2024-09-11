// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Modules/Unit/UnitInfoConfigTableRow.h"
#include "Modules/Exp/ExpTypeConfigTableRow.h"
#include "Modules/Assets/BackpackTypeConfigTableRow.h"
#include "Modules/Item/ItemConfigTableRow.h"
#include "Modules/Item/ItemTypeConfigTableRow.h"
#include "Modules/Skill/SkillGroupConfigTableRow.h"
#include "EditorCommon.h"

/**
 * 
 */
class GAMEFRAMEWORKEDITOR_API GameFrameworkEditorWidgetTool
{
public:
	void ShowNotification(const FText& TextToDisplay, float TimeToDisplay);
	TArray<TSharedPtr<FConfigTableRowWrapper>> GetUnitInfoSource();
    TArray<TSharedPtr<FConfigTableRowWrapper>> GetUnitGroupSource();
	TArray<TSharedPtr<FConfigTableRowWrapper>> GetExpTypeSource();
	TArray<TSharedPtr<FConfigTableRowWrapper>> GetBackpackTypeSource();
	TArray<TSharedPtr<FConfigTableRowWrapper>> GetItemInfoSource();
	TArray<TSharedPtr<FConfigTableRowWrapper>> GetItemTypeSource();
	TArray<TSharedPtr<FConfigTableRowWrapper>> GetSkillSource();
	TArray<TSharedPtr<FConfigTableRowWrapper>> GetEffectSource();
    TArray<TSharedPtr<FConfigTableRowWrapper>> GetSkillGroupSource();

	virtual bool IsPackageTypeIdUse(int32 BackpackTypeId, FString& UseInfo);
	virtual bool IsItemIdUse(int32 ItemId, FString& UseInfo);
	virtual bool IsUnitIdUse(int32 UnitId, FString& UseInfo);
    virtual bool IsSkillGroupIdUse(int32 SkillId, FString& UseInfo);
	virtual bool IsItemIdExist(int32 ItemId);
	virtual bool IsUnitIdExist(int32 UnitId);

	static void ExportEnumBP(FString EnumClassName, FText EnumDisplayName, TMap<int64, TPair<FString, FText>> EnumSource);
};