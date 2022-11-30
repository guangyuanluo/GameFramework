// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillGroupConfigTableRow.h"

int FSkillGroupConfigTableRow::GetUniqueId() {
    return SkillGroupID;
}

FString FSkillGroupConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { SkillGroupID, Description });
}