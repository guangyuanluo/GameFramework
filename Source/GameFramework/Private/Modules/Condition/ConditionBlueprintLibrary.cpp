// Fill out your copyright notice in the Description page of Project Settings.

#include "ConditionBlueprintLibrary.h"
#include "CoreConditionProgress.h"
#include "BooleanAlgebraUtil.h"

bool UConditionBlueprintLibrary::DoesProgressesSatisfy(const TArray<class UCoreConditionProgress*>& Progresses) {
	bool HaveComplete = true;

	TArray<BooleanAlgebraEnum> LoopGroupRelations;
	for (int ProgressIndex = 0; ProgressIndex < Progresses.Num(); ++ProgressIndex) {
		if (!Progresses[ProgressIndex]) {
			return false;
		}
		LoopGroupRelations.Add(Progresses[ProgressIndex]->Condition->Relation);
	}
	FBooleanAlgebraNodeInfo GroupExecuteRoot = UBooleanAlgebraUtil::RelationsGenerate(LoopGroupRelations);
	TFunction<bool(int)> CheckFunction = [&Progresses](int ProgressIndex) {
		auto Progress = Progresses[ProgressIndex];
		auto Condition = Progress->Condition;

		bool IsValid;
		bool bLastSatisfy = Progress->IsComplete(IsValid);
		return IsValid && bLastSatisfy != Condition->bNot;
	};

	HaveComplete = UBooleanAlgebraUtil::ExecuteConditionRelationTree(GroupExecuteRoot, CheckFunction);

	return HaveComplete;
}