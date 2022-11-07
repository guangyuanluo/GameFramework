// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "QuestDetailGraphSchema.h"
#include "Textures/SlateIcon.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphPin.h"
#include "Graph/GameFrameworkGraphNode.h"
#include "GraphEditorActions.h"
#include "ScopedTransaction.h"
#include "Framework/Commands/GenericCommands.h"
#include "GameFrameworkEditor.h"
#include "Graph/GameFrameworkGraphTypes.h"
#include "Graph/GameSchemaAction_NewNode.h"
#include "ToolMenus.h"
#include "Modules/Quest/QuestDetailNode.h"
#include "QuestDetailGraphNode.h"
#include "QuestDetailGraphNode_Root.h"

#include "Graph/ArrowConnectionDrawingPolicy.h"

#define LOCTEXT_NAMESPACE "QuestTreeGraph"

UQuestDetailGraphSchema::UQuestDetailGraphSchema(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UQuestDetailGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UQuestDetailGraphNode_Root> NodeCreator(Graph);
    UQuestDetailGraphNode_Root* MyNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(MyNode, FNodeMetadata::DefaultGraphNode);
}

void UQuestDetailGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<struct FGraphNodeClassHelper> ClassCache = MakeShareable(new FGraphNodeClassHelper(UQuestDetailNode::StaticClass()));
	ClassCache->UpdateAvailableBlueprintClasses();

	FCategorizedGraphActionListBuilder QuestBuilder(TEXT("QuestDetailNode"));

	TArray<FGameFrameworkGraphNodeClassData> NodeClasses;
	ClassCache->GatherClasses(UQuestDetailNode::StaticClass(), NodeClasses);

	for (auto& NodeClass : NodeClasses)
	{
        if (NodeClass.GetClass()->HasAnyClassFlags(CLASS_Abstract)) continue;
        UQuestDetailNode* QuestDetailNode = Cast<UQuestDetailNode>(NodeClass.GetClass()->GetDefaultObject());
        const FText NodeTypeName = FText::FromString(QuestDetailNode->GetNodeTypeName());

        TSharedPtr<FGameSchemaAction_NewNode> AddOpAction = UQuestDetailGraphSchema::AddNewNodeAction(QuestBuilder, NodeClass.GetCategory(), NodeTypeName, FText::GetEmpty());

        UGameFrameworkGraphNode* OpNode = NewObject<UGameFrameworkGraphNode>(ContextMenuBuilder.OwnerOfTemporaries, UQuestDetailGraphNode::StaticClass());
        OpNode->NodeTitle = QuestDetailNode->GetNodeTypeName();
        OpNode->ClassData = NodeClass;
        AddOpAction->NodeTemplate = OpNode;
	}

	ContextMenuBuilder.Append(QuestBuilder);
}

const FPinConnectionResponse UQuestDetailGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	{
		if (A->GetOwningNode() == B->GetOwningNode())
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Both are on the same node"));
		}
	}

	// Compare the directions
	if ((A->Direction == EGPD_Input) && (B->Direction == EGPD_Input))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorInput", "Can't connect input node to input node"));
	}
	else if ((B->Direction == EGPD_Output) && (A->Direction == EGPD_Output))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorOutput", "Can't connect output node to output node"));
	}
    // check allow
    const UEdGraphPin* InputPin = nullptr;
    const UEdGraphPin* OutputPin = nullptr;
    
    UGameFrameworkGraphNode* InputNode = nullptr;
    UGameFrameworkGraphNode* OutputNode = nullptr;
    bool isoutputb = false;
    if (A->Direction == EGPD_Input) {
        InputPin = A;
        OutputPin = B;
        isoutputb = true;
        InputNode = Cast<UGameFrameworkGraphNode>(A->GetOwningNode());
        OutputNode = Cast<UGameFrameworkGraphNode>(B->GetOwningNode());
    }
    else {
        InputPin = B;
        OutputPin = A;
        InputNode = Cast<UQuestDetailGraphNode>(B->GetOwningNode());
        OutputNode = Cast<UQuestDetailGraphNode>(A->GetOwningNode());
    }
    if (!InputNode->AllowOutputPin(OutputNode->GetClass())) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString(TEXT("输入节点不允许这样的输出节点")));
    }
    if (!OutputNode->AllowInputPin(InputNode->GetClass())) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString(TEXT("输出节点不允许这样的输入节点")));
    }
    if (OutputPin->PinType.PinCategory == UGameFrameworkGraphNode::PinCategory_SingleNode) {
        if (InputPin->PinType.PinCategory == UGameFrameworkGraphNode::PinCategory_SingleNode) {
            return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, LOCTEXT("PinConnectReplace", "Replace connection"));
        }
        else {
            if (isoutputb) {
                return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, LOCTEXT("PinConnectReplace", "Replace connection"));
            }
            else {
                return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, LOCTEXT("PinConnectReplace", "Replace connection"));
            }
        }
    }
    else {
        return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("ConnectResponse_Allowed", "Connect"));
    }

	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorInput", "Can't connect input node to input node"));
}

TSharedPtr<FGameSchemaAction_NewNode> UQuestDetailGraphSchema::AddNewNodeAction(FGraphActionListBuilderBase& ContextMenuBuilder, const FText& Category, const FText& MenuDesc, const FText& Tooltip)
{
	TSharedPtr<FGameSchemaAction_NewNode> NewAction = TSharedPtr<FGameSchemaAction_NewNode>(new FGameSchemaAction_NewNode(Category, MenuDesc, Tooltip, 0));
	ContextMenuBuilder.AddAction(NewAction);

	return NewAction;
}

class FConnectionDrawingPolicy* UQuestDetailGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const {
    return new FArrowConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

void UQuestDetailGraphSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const {
    if (Context->Node) {
        {
            FToolMenuSection& Section = Menu->AddSection("QuestDetailGraphSchemaNodeActions", LOCTEXT("ClassActionsMenuHeader", "Node Actions"));
            Section.AddMenuEntry(FGenericCommands::Get().Delete);
            //Section.AddMenuEntry(FGenericCommands::Get().Cut);
            //Section.AddMenuEntry(FGenericCommands::Get().Copy);
            //Section.AddMenuEntry(FGenericCommands::Get().Duplicate);

            Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
        }
    }

    Super::GetContextMenuActions(Menu, Context);
}


#undef LOCTEXT_NAMESPACE
