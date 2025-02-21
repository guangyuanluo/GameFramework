// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/Condition/CoreCondition.h"
#include "Modules/Condition/CoreConditionProgress.h"
#include "CoreConditionGroup.generated.h"

/**
 * 条件组
 */
UCLASS(BlueprintType, meta = (DisplayName = "条件组"))
class GAMEFRAMEWORK_API UCoreConditionGroup : public UCoreCondition {
public:
	GENERATED_UCLASS_BODY()

	/**
	* @brief 子条件
	*/
	UPROPERTY(Category = "ConditionSystem", EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<TObjectPtr<class UCoreCondition>> ConditionList;

	virtual class UCoreConditionProgress* GenerateConditionProgress(AActor* ProgressOwner) override;
};

/**
 * 条件组进度
 */
UCLASS(BlueprintType)
class GAMEFRAMEWORK_API UCoreConditionGroupProgress : public UCoreConditionProgress {
public:
	GENERATED_BODY()

	virtual void Initialize() override;
	virtual void Uninitialize() override;
	virtual bool IsComplete(bool& IsValid) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void GetProgressesWithChildren(TArray<UCoreConditionProgress*>& OutProgresses) override;

private:
	friend class UCoreConditionGroup;

	UPROPERTY(ReplicatedUsing = OnRep_ChildProgresses)
	TArray<UCoreConditionProgress*> ChildProgresses;

	UFUNCTION()
	void OnChildConditionSatisfyChange(UCoreConditionProgress* Progress, bool NewSatisfy);

	UFUNCTION()
	void OnChildProgressPostNetReceive(UCoreConditionProgress* Progress);

	UFUNCTION()
	void OnRep_ChildProgresses(const TArray<UCoreConditionProgress*>& OldChildProgresses);
};