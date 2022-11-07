// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphNode.h"
#include "GameFrameworkGraphTypes.h"
#include "GameFrameworkGraphNode.generated.h"

class UEdGraph;
class UEdGraphPin;
class UEdGraphSchema;
struct FDiffResults;
struct FDiffSingleResult;

UCLASS()
class UGameFrameworkGraphNode : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()

public:
	static const FName PinCategory_MultipleNodes;
	static const FName PinCategory_SingleNode;


	/** instance class */
	UPROPERTY()
	struct FGameFrameworkGraphNodeClassData ClassData;

	UPROPERTY()
	UObject* NodeInstance;

	UPROPERTY()
	FString NodeTitle;

	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual void PostPlacedNewNode() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void NodeConnectionListChanged() override;
	// @return the input pin for this state
	virtual UEdGraphPin* GetInputPin(int32 InputIndex = 0) const;
	// @return the output pin for this state
	virtual UEdGraphPin* GetOutputPin(int32 InputIndex = 0) const;

	/** initialize instance object  */
	virtual void InitializeInstance();
	virtual bool AllowInputPin(TSubclassOf<UEdGraphNode> NodeClass);
	virtual bool AllowOutputPin(TSubclassOf<UEdGraphNode> NodeClass);
	virtual void RefreshNodeOnDataChanged() {}
	virtual void PrepareForCopying() override;
	virtual void PostCopyNode();

	//~ Begin UObject Interface
#if WITH_EDITOR
	virtual void PostEditImport() override;
#endif
	// End UObject

	virtual void ResetNodeOwner();
};
