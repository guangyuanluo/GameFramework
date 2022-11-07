// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkillInfo.h"
#include "ConfigTableCache.h"
#include "SkillSetting.h"

bool USkillInfoUtil::IsValid(const FSkillInfo& SkillInfo) {
    const USkillSetting* SkillSetting = GetDefault<USkillSetting>();
    auto SkillDataTable = SkillSetting->SkillTable.LoadSynchronous();
    if (SkillDataTable) {
        return UConfigTableCache::GetDataTableRawDataFromId(SkillDataTable, SkillInfo.SkillId) != nullptr;
    }

    return false;
}