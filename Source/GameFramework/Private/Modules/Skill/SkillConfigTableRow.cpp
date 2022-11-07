// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillConfigTableRow.h"

int FSkillConfigTableRow::GetUniqueId() {
    return SkillId;
}

FString FSkillConfigTableRow::GetSimpleDescription() {
    return FString::Format(TEXT("{0}.{1}"), { SkillId, Description });
}