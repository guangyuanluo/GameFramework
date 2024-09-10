// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCSystem.h"
#include "CoreCharacter.h"
#include "GameFrameworkUtils.h"

void UNPCSystem::AddNPC(ACoreCharacter* NPC) {
    check(!NPCMap.Contains(NPC->TemplateID));
    auto& NPCInfo = NPCMap.FindOrAdd(NPC->TemplateID);
    NPCInfo.NPC = NPC;
}

void UNPCSystem::RemoveNPC(ACoreCharacter* NPC) {
    auto FindNPCInfoPtr = NPCMap.Find(NPC->TemplateID);
    if (FindNPCInfoPtr) {
        NPCMap.Remove(NPC->TemplateID);
    }
}

bool UNPCSystem::FindNPCInfo(int32 NPCID, FNPCInfo& NPCInfo) {
    auto FindNPCInfoPtr = NPCMap.Find(NPCID);
    if (FindNPCInfoPtr) {
        NPCInfo = *FindNPCInfoPtr;
        return true;
    }
    return false;
}