// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCSystem.h"
#include "CoreCharacter.h"
#include "GameFrameworkUtils.h"
#include "CoreGameInstance.h"
#include "GameSystemManager.h"
#include "EventSystem.h"
#include "NPCAcquireEvent.h"
#include "NPCReleaseEvent.h"

void UNPCSystem::AddNPC(ACoreCharacter* NPC) {
    check(!NPCMap.Contains(NPC->TemplateID));
    auto& NPCInfo = NPCMap.FindOrAdd(NPC->TemplateID);
    NPCInfo.NPC = NPC;
}

void UNPCSystem::RemoveNPC(ACoreCharacter* NPC) {
    auto FindNPCInfoPtr = NPCMap.Find(NPC->TemplateID);
    if (FindNPCInfoPtr) {
        if (FindNPCInfoPtr->IsReleased) {
            //未被占有就移除
            NPCMap.Remove(NPC->TemplateID);
        }
        else {
            // NPC 被占有
            FindNPCInfoPtr->NPC = nullptr;
        }
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

bool UNPCSystem::TryAcquireNPCByID(AActor* Owner, const TArray<int32>& NPCIDs, UObject* CustomInfo) {
    if (!IsNPCReleaseByID(NPCIDs)) {
        return false;
    }
    for (int Index = 0; Index < NPCIDs.Num(); ++Index) {
        auto NPCID = NPCIDs[Index];
        auto& FindNPCInfo = NPCMap.FindOrAdd(NPCID);
        FindNPCInfo.IsReleased = false;
        FindNPCInfo.CustomInfo = CustomInfo;
    }
    //占有事件
    UNPCAcquireEvent* NPCAcquireEvent = NewObject<UNPCAcquireEvent>();
    NPCAcquireEvent->Source = Owner;
    NPCAcquireEvent->NPCIDs = NPCIDs;
    NPCAcquireEvent->CustomInfo = CustomInfo;
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(NPCAcquireEvent);
    return true;
}

bool UNPCSystem::TryAcquireNPCByContainer(AActor* Owner, const TArray<FUnitIDContainer>& NPCIDContainers, UObject* CustomInfo) {
    TArray<int32> UnitIDs;
    for (int Index = 0; Index < NPCIDContainers.Num(); ++Index) {
        auto NPCID = NPCIDContainers[Index].UnitID;
        UnitIDs.Add(NPCID);
    }
    return TryAcquireNPCByID(Owner, UnitIDs, CustomInfo);
}

bool UNPCSystem::IsNPCReleaseByID(const TArray<int32>& NPCIDs) {
    for (int Index = 0; Index < NPCIDs.Num(); ++Index) {
        auto NPCID = NPCIDs[Index];
        auto FindNPCInfo = NPCMap.Find(NPCID);
        if (FindNPCInfo != nullptr
            && !FindNPCInfo->IsReleased) {
            return false;
        }
    }
    return true;
}

bool UNPCSystem::IsNPCReleaseByContainer(const TArray<FUnitIDContainer>& NPCIDs) {
    for (int Index = 0; Index < NPCIDs.Num(); ++Index) {
        auto NPCID = NPCIDs[Index].UnitID;
        auto FindNPCInfo = NPCMap.Find(NPCID);
        if (FindNPCInfo != nullptr
            && !FindNPCInfo->IsReleased) {
            return false;
        }
    }
    return true;
}

void UNPCSystem::ReleaseNPCByID(AActor* Owner, const TArray<int32>& NPCIDs) {
    for (int Index = 0; Index < NPCIDs.Num(); ++Index) {
        auto NPCID = NPCIDs[Index];
        auto FindNPCInfo = NPCMap.Find(NPCID);
        if (FindNPCInfo) {
            FindNPCInfo->IsReleased = true;
            FindNPCInfo->CustomInfo = nullptr;
            if (!FindNPCInfo->NPC) {
                //NPC已经释放，这里删除map
                NPCMap.Remove(NPCID);
            }
        }
    }
    //释放事件
    UNPCReleaseEvent* NPCReleaseEvent = NewObject<UNPCReleaseEvent>();
    NPCReleaseEvent->Source = Owner;
    NPCReleaseEvent->NPCIDs = NPCIDs;
    GameInstance->GameSystemManager->GetSystemByClass<UEventSystem>()->PushEvent(NPCReleaseEvent);
}

void UNPCSystem::ReleaseNPCByContainer(AActor* Owner, const TArray<FUnitIDContainer>& NPCIDContainers) {
    TArray<int32> UnitIDs;
    for (int Index = 0; Index < NPCIDContainers.Num(); ++Index) {
        auto NPCID = NPCIDContainers[Index].UnitID;
        UnitIDs.Add(NPCID);
    }
    ReleaseNPCByID(Owner, UnitIDs);
}