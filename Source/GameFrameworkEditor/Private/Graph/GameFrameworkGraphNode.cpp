// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Graph/GameFrameworkGraphNode.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "Engine/Blueprint.h"
#include "AssetRegistry/AssetData.h"
#include "EdGraph/EdGraphSchema.h"
#include "Graph/GameFrameworkGraph.h"
#include "DiffResults.h"
#include "ScopedTransaction.h"
#include "BlueprintNodeHelpers.h"
#include "Modules/Scenario/ScenarioNode.h"

#define LOCTEXT_NAMESPACE "ScenarioGraph"

const FName UGameFrameworkGraphNode::PinCategory_MultipleNodes("MultipleNodes");
const FName UGameFrameworkGraphNode::PinCategory_SingleNode("SingleNode");

UGameFrameworkGraphNode::UGameFrameworkGraphNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeInstance = nullptr;
}

void UGameFrameworkGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, PinCategory_MultipleNodes, TEXT("In"));
	CreatePin(EGPD_Output, PinCategory_MultipleNodes, TEXT("Out"));
}

void UGameFrameworkGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != nullptr)
	{
		UEdGraphPin* OutputPin = GetOutputPin();

		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
		else if (OutputPin != nullptr && GetSchema()->TryCreateConnection(OutputPin, FromPin))
		{
			NodeConnectionListChanged();
		}
	}
}

void UGameFrameworkGraphNode::PostPlacedNewNode()
{
	// NodeInstance can be already spawned by paste operation, don't override it

	UClass* NodeClass = ClassData.GetClass(true);
	if (NodeClass && (NodeInstance == nullptr))
	{
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;
		if (GraphOwner)
		{
			NodeInstance = NewObject<UObject>(GraphOwner, NodeClass);
			NodeInstance->SetFlags(RF_Transactional);
			InitializeInstance();
		}
	}
}

FText UGameFrameworkGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(NodeTitle);
}

void UGameFrameworkGraphNode::NodeConnectionListChanged()
{
	Cast<UGameFrameworkGraph>(GetGraph())->NotifyGraphPinConnectionChanged();
}

UEdGraphPin* UGameFrameworkGraphNode::GetInputPin(int32 InputIndex) const
{
	check(InputIndex >= 0);

	for (int32 PinIndex = 0, FoundInputs = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex]->Direction == EGPD_Input)
		{
			if (InputIndex == FoundInputs)
			{
				return Pins[PinIndex];
			}
			else
			{
				FoundInputs++;
			}
		}
	}

	return nullptr;
}

UEdGraphPin* UGameFrameworkGraphNode::GetOutputPin(int32 InputIndex) const
{
	check(InputIndex >= 0);

	for (int32 PinIndex = 0, FoundInputs = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex]->Direction == EGPD_Output)
		{
			if (InputIndex == FoundInputs)
			{
				return Pins[PinIndex];
			}
			else
			{
				FoundInputs++;
			}
		}
	}

	return nullptr;
}

void UGameFrameworkGraphNode::InitializeInstance()
{

}

bool UGameFrameworkGraphNode::AllowInputPin(TSubclassOf<class UEdGraphNode> NodeClass) {
	return true;
}

bool UGameFrameworkGraphNode::AllowOutputPin(TSubclassOf<class UEdGraphNode> NodeClass) {
	return true;
}

void UGameFrameworkGraphNode::PrepareForCopying() {
	if (NodeInstance) {
		// Temporarily take ownership of the node instance, so that it is not deleted when cutting
		NodeInstance->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
	}
}

void UGameFrameworkGraphNode::PostCopyNode() {
	ResetNodeOwner();
}

#if WITH_EDITOR

void UGameFrameworkGraphNode::PostEditImport() {
	ResetNodeOwner();

	if (NodeInstance) {
		InitializeInstance();
	}
}

#endif

void UGameFrameworkGraphNode::ResetNodeOwner() {
	if (NodeInstance) {
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;

		NodeInstance->Rename(NULL, GraphOwner, REN_DontCreateRedirectors | REN_DoNotDirty);
		NodeInstance->ClearFlags(RF_Transient);
	}
}

#undef LOCTEXT_NAMESPACE
