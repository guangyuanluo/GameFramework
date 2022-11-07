// Copyright Epic Games, Inc. All Rights Reserved.

#include "Base/ConfigTable/K2Node_GetConfigTableRow.h"
#include "Base/ConfigTable/ConfigTableCache.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_IfThenElse.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "DataTableEditorUtils.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"

#define LOCTEXT_NAMESPACE "K2Node_GetDataTableRow"

namespace GetDataTableRowHelper
{

const FName DataTablePinName = "DataTable";
const FName RowNotFoundPinName = "RowNotFound";
const FName IdPinName = "Id";

}

UK2Node_GetConfigTableRow::UK2Node_GetConfigTableRow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Attempts to retrieve a TableRow from a DataTable via it's Id");
}

void UK2Node_GetConfigTableRow::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* RowFoundPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	RowFoundPin->PinFriendlyName = LOCTEXT("GetDataTableRow Row Found Exec pin", "Row Found");
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetDataTableRowHelper::RowNotFoundPinName);

	// Add DataTable pin
	UEdGraphPin* DataTablePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UDataTable::StaticClass(), GetDataTableRowHelper::DataTablePinName);
	SetPinToolTip(*DataTablePin, LOCTEXT("DataTablePinDescription", "The DataTable you want to retreive a Row from"));

	// Id pin
	UEdGraphPin* IdPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, GetDataTableRowHelper::IdPinName);
	SetPinToolTip(*IdPin, LOCTEXT("RowNamePinDescription", "The id to retrieve from the DataTable"));

	// Result pin
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UEdGraphSchema_K2::PN_ReturnValue);
	ResultPin->PinFriendlyName = LOCTEXT("GetDataTableRow Output Row", "Out Row");
	SetPinToolTip(*ResultPin, LOCTEXT("ResultPinDescription", "The returned TableRow, if found"));

	Super::AllocateDefaultPins();
}

void UK2Node_GetConfigTableRow::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
{
	MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

	UEdGraphSchema_K2 const* const K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema());
	if (K2Schema != nullptr)
	{
		MutatablePin.PinToolTip += TEXT(" ");
		MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
	}

	MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

void UK2Node_GetConfigTableRow::RefreshOutputPinType()
{
	UScriptStruct* OutputType = GetDataTableRowStructType();
	SetReturnTypeForStruct(OutputType);
}

void UK2Node_GetConfigTableRow::RefreshRowNameOptions()
{
	// When the DataTable pin gets a new value assigned, we need to update the Slate UI so that SGraphNodeCallParameterCollectionFunction will update the ParameterName drop down
	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}


void UK2Node_GetConfigTableRow::SetReturnTypeForStruct(UScriptStruct* NewRowStruct)
{
	UScriptStruct* OldRowStruct = GetReturnTypeForStruct();
	if (NewRowStruct != OldRowStruct)
	{
		UEdGraphPin* ResultPin = GetResultPin();

		if (ResultPin->SubPins.Num() > 0)
		{
			GetSchema()->RecombinePin(ResultPin);
		}

		// NOTE: purposefully not disconnecting the ResultPin (even though it changed type)... we want the user to see the old
		//       connections, and incompatible connections will produce an error (plus, some super-struct connections may still be valid)
		ResultPin->PinType.PinSubCategoryObject = NewRowStruct;
		ResultPin->PinType.PinCategory = (NewRowStruct == nullptr) ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;

		CachedNodeTitle.Clear();
	}
}

UScriptStruct* UK2Node_GetConfigTableRow::GetReturnTypeForStruct()
{
	UScriptStruct* ReturnStructType = (UScriptStruct*)(GetResultPin()->PinType.PinSubCategoryObject.Get());

	return ReturnStructType;
}

UScriptStruct* UK2Node_GetConfigTableRow::GetDataTableRowStructType() const
{
	UScriptStruct* RowStructType = nullptr;

	UEdGraphPin* DataTablePin = GetDataTablePin();
	if(DataTablePin && DataTablePin->DefaultObject != nullptr && DataTablePin->LinkedTo.Num() == 0)
	{
		if (const UDataTable* DataTable = Cast<const UDataTable>(DataTablePin->DefaultObject))
		{
			RowStructType = DataTable->RowStruct;
		}
	}

	if (RowStructType == nullptr)
	{
		UEdGraphPin* ResultPin = GetResultPin();
		if (ResultPin && ResultPin->LinkedTo.Num() > 0)
		{
			RowStructType = Cast<UScriptStruct>(ResultPin->LinkedTo[0]->PinType.PinSubCategoryObject.Get());
			for (int32 LinkIndex = 1; LinkIndex < ResultPin->LinkedTo.Num(); ++LinkIndex)
			{
				UEdGraphPin* Link = ResultPin->LinkedTo[LinkIndex];
				UScriptStruct* LinkType = Cast<UScriptStruct>(Link->PinType.PinSubCategoryObject.Get());

				if (RowStructType->IsChildOf(LinkType))
				{
					RowStructType = LinkType;
				}
			}
		}
	}
	return RowStructType;
}

void UK2Node_GetConfigTableRow::OnDataTableRowListChanged(const UDataTable* DataTable)
{
	UEdGraphPin* DataTablePin = GetDataTablePin();
	if (DataTable && DataTablePin && DataTable == DataTablePin->DefaultObject)
	{
		UEdGraphPin* IdPin = GetIdPin();
		const bool TryRefresh = IdPin && !IdPin->LinkedTo.Num();
		int32 CurrentId = IdPin ? FCString::Atoi(*IdPin->DefaultValue) : 0;
		if (TryRefresh && IdPin && !DataTableContainsId(DataTable, CurrentId))
		{
			if (UBlueprint* BP = GetBlueprint())
			{
				FBlueprintEditorUtils::MarkBlueprintAsModified(BP);
			}
		}
	}
}

void UK2Node_GetConfigTableRow::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);

	if (UEdGraphPin* DataTablePin = GetDataTablePin(&OldPins))
	{
		if (UDataTable* DataTable = Cast<UDataTable>(DataTablePin->DefaultObject))
		{
			// make sure to properly load the data-table object so that we can 
			// farm the "RowStruct" property from it (below, in GetDataTableRowStructType)
			PreloadObject(DataTable);
		}
	}
}

void UK2Node_GetConfigTableRow::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_GetConfigTableRow::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}

bool UK2Node_GetConfigTableRow::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin == GetResultPin() && MyPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
	{
		bool bDisallowed = true;
		if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
		{
			if (UScriptStruct* ConnectionType = Cast<UScriptStruct>(OtherPin->PinType.PinSubCategoryObject.Get()))
			{
				bDisallowed = !FDataTableEditorUtils::IsValidTableStruct(ConnectionType);
			}
		}
		else if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
		{
			bDisallowed = false;
		}

		if (bDisallowed)
		{
			OutReason = TEXT("Must be a struct that can be used in a DataTable");
		}
		return bDisallowed;
	}
	return false;
}

void UK2Node_GetConfigTableRow::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	if (ChangedPin && ChangedPin->PinName == GetDataTableRowHelper::DataTablePinName)
	{
		RefreshOutputPinType();

		UEdGraphPin* IdPin = GetIdPin();
		UDataTable*  DataTable = Cast<UDataTable>(ChangedPin->DefaultObject);
		if (IdPin)
		{
            int32 CurrentId = IdPin ? FCString::Atoi(*IdPin->DefaultValue) : 0;
			if (DataTable && (IdPin->DefaultValue.IsEmpty() || !DataTableContainsId(DataTable, CurrentId)))
			{
				if (auto Iterator = DataTable->GetRowMap().CreateConstIterator())
				{
					IdPin->DefaultValue = FString::FromInt(((FConfigTableRowBase*)(Iterator.Value()))->GetUniqueId());
				}
			}	

			RefreshRowNameOptions();
		}
	}
}

FText UK2Node_GetConfigTableRow::GetTooltipText() const
{
	return NodeTooltip;
}

UEdGraphPin* UK2Node_GetConfigTableRow::GetThenPin()const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_GetConfigTableRow::GetDataTablePin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;
    
	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* TestPin : *PinsToSearch)
	{
		if (TestPin && TestPin->PinName == GetDataTableRowHelper::DataTablePinName)
		{
			Pin = TestPin;
			break;
		}
	}
	check(Pin == nullptr || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetConfigTableRow::GetIdPin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::IdPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetConfigTableRow::GetRowNotFoundPin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::RowNotFoundPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_GetConfigTableRow::GetResultPin() const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

FText UK2Node_GetConfigTableRow::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return LOCTEXT("ListViewTitle", "Get Data Table Row");
	}
	else if (UEdGraphPin* DataTablePin = GetDataTablePin())
	{
		if (DataTablePin->LinkedTo.Num() > 0)
		{
			return NSLOCTEXT("K2Node", "DataTable_Title_Unknown", "Get Data Table Row");
		}
		else if (DataTablePin->DefaultObject == nullptr)
		{
			return NSLOCTEXT("K2Node", "DataTable_Title_None", "Get Data Table Row NONE");
		}
		else if (CachedNodeTitle.IsOutOfDate(this))
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("DataTableName"), FText::FromString(DataTablePin->DefaultObject->GetName()));

			FText LocFormat = NSLOCTEXT("K2Node", "DataTable", "Get Data Table Row {DataTableName}");
			// FText::Format() is slow, so we cache this to save on performance
			CachedNodeTitle.SetCachedText(FText::Format(LocFormat, Args), this);
		}
	}
	else
	{
		return NSLOCTEXT("K2Node", "DataTable_Title_None", "Get Data Table Row NONE");
	}	
	return CachedNodeTitle;
}

void UK2Node_GetConfigTableRow::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);
    
    UEdGraphPin* OriginalDataTableInPin = GetDataTablePin();
    UDataTable* Table = (OriginalDataTableInPin != NULL) ? Cast<UDataTable>(OriginalDataTableInPin->DefaultObject) : NULL;
    if((nullptr == OriginalDataTableInPin) || (0 == OriginalDataTableInPin->LinkedTo.Num() && nullptr == Table))
    {
        CompilerContext.MessageLog.Error(*LOCTEXT("GetDataTableRowNoDataTable_Error", "GetDataTableRow must have a DataTable specified.").ToString(), this);
        // we break exec links so this is the only error we get
        BreakAllNodeLinks();
        return;
    }

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UConfigTableCache, GetDataTableRowFromId);
	UK2Node_CallFunction* GetDataTableRowFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	GetDataTableRowFunction->FunctionReference.SetExternalMember(FunctionName, UDataTableFunctionLibrary::StaticClass());
	GetDataTableRowFunction->AllocateDefaultPins();
    CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(GetDataTableRowFunction->GetExecPin()));

	// Connect the input of our GetDataTableRow to the Input of our Function pin
    UEdGraphPin* DataTableInPin = GetDataTableRowFunction->FindPinChecked(TEXT("Table"));
	if(OriginalDataTableInPin->LinkedTo.Num() > 0)
	{
		// Copy the connection
		CompilerContext.MovePinLinksToIntermediate(*OriginalDataTableInPin, *DataTableInPin);
	}
	else
	{
		// Copy literal
		DataTableInPin->DefaultObject = OriginalDataTableInPin->DefaultObject;
	}
	UEdGraphPin* IdInPin = GetDataTableRowFunction->FindPinChecked(GetDataTableRowHelper::IdPinName);
	CompilerContext.MovePinLinksToIntermediate(*GetIdPin(), *IdInPin);

	// Get some pins to work with
	UEdGraphPin* OriginalOutRowPin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	UEdGraphPin* FunctionOutRowPin = GetDataTableRowFunction->FindPinChecked(TEXT("OutRow"));
    UEdGraphPin* FunctionReturnPin = GetDataTableRowFunction->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    UEdGraphPin* FunctionThenPin = GetDataTableRowFunction->GetThenPin();
        
    // Set the type of the OutRow pin on this expanded mode to match original
    FunctionOutRowPin->PinType = OriginalOutRowPin->PinType;
	FunctionOutRowPin->PinType.PinSubCategoryObject = OriginalOutRowPin->PinType.PinSubCategoryObject;
        
    //BRANCH NODE
    UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
    BranchNode->AllocateDefaultPins();
    // Hook up inputs to branch
    FunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
    FunctionReturnPin->MakeLinkTo(BranchNode->GetConditionPin());
        
    // Hook up outputs
    CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
    CompilerContext.MovePinLinksToIntermediate(*GetRowNotFoundPin(), *(BranchNode->GetElsePin()));
    CompilerContext.MovePinLinksToIntermediate(*OriginalOutRowPin, *FunctionOutRowPin);

	BreakAllNodeLinks();
}

FSlateIcon UK2Node_GetConfigTableRow::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_GetConfigTableRow::PostReconstructNode()
{
	Super::PostReconstructNode();

	RefreshOutputPinType();
}

void UK2Node_GetConfigTableRow::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);

	const UEdGraphPin* DataTablePin = GetDataTablePin();
	const UEdGraphPin* IdPin = GetIdPin();
	if (!DataTablePin || !IdPin)
	{
		MessageLog.Error(*LOCTEXT("MissingPins", "Missing pins in @@").ToString(), this);
		return;
	}

	if (DataTablePin->LinkedTo.Num() == 0)
	{
		const UDataTable* DataTable = Cast<UDataTable>(DataTablePin->DefaultObject);
		if (!DataTable)
		{
			MessageLog.Error(*LOCTEXT("NoDataTable", "No DataTable in @@").ToString(), this);
			return;
		}

		if (!IdPin->LinkedTo.Num())
		{
            int32 CurrentId = IdPin ? FCString::Atoi(*IdPin->DefaultValue) : 0;
			if (!DataTableContainsId(DataTable, CurrentId))
			{
				const FString Msg = FText::Format(
					LOCTEXT("WrongRowNameFmt", "'{0}' Row name is not stored in '{1}'. @@"),
					FText::FromString(FString::FromInt(CurrentId)),
					FText::FromString(GetFullNameSafe(DataTable))
				).ToString();
				MessageLog.Error(*Msg, this);
				return;
			}
		}
	}	
}

void UK2Node_GetConfigTableRow::PreloadRequiredAssets()
{
	if (UEdGraphPin* DataTablePin = GetDataTablePin())
	{
		if (UDataTable* DataTable = Cast<UDataTable>(DataTablePin->DefaultObject))
		{
			// make sure to properly load the data-table object so that we can 
			// farm the "RowStruct" property from it (below, in GetDataTableRowStructType)
			PreloadObject(DataTable);
		}
	}
	return Super::PreloadRequiredAssets();
}

void UK2Node_GetConfigTableRow::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetResultPin())
	{
		UEdGraphPin* TablePin = GetDataTablePin();
		// this connection would only change the output type if the table pin is undefined
		const bool bIsTypeAuthority = (TablePin->LinkedTo.Num() > 0 || TablePin->DefaultObject == nullptr);
		if (bIsTypeAuthority)
		{
			RefreshOutputPinType();
		}		
	}
	else if (Pin == GetDataTablePin())
	{
		const bool bConnectionAdded = Pin->LinkedTo.Num() > 0;
		if (bConnectionAdded)
		{
			// if a connection was made, then we may need to rid ourselves of the Row dropdown
			RefreshRowNameOptions();
			// if the output connection was previously, incompatible, it now becomes the authority on this node's output type
			RefreshOutputPinType();
		}
	}
}

bool UK2Node_GetConfigTableRow::DataTableContainsId(const UDataTable* DataTable, int Id) const
{
    TArray<FConfigTableRowBase*> ConfigTableRows;
    DataTable->GetAllRows("", ConfigTableRows);
    for (int Index = 0; Index < ConfigTableRows.Num(); ++Index) {
        auto ConfigTableRowPtr = ConfigTableRows[Index];
        if (ConfigTableRowPtr->GetUniqueId() == Id) {
            return true;
        }
    }
    return false;
}

#undef LOCTEXT_NAMESPACE
