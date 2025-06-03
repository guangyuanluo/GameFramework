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

		EConditionSatisfy LastSatisfy = Progress->GetSatisfy();
		if (LastSatisfy == EConditionSatisfy::ECS_Unknown)
		{
			return false;
		}
		EConditionSatisfy SatisfyExpected = Condition->bNot ? EConditionSatisfy::ECS_NotSatisfy : EConditionSatisfy::ECS_Satisfy;
		return SatisfyExpected == LastSatisfy;
	};

	HaveComplete = UBooleanAlgebraUtil::ExecuteConditionRelationTree(GroupExecuteRoot, CheckFunction);

	return HaveComplete;
}