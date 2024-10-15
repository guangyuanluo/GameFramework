// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Base/ConfigTable/ConfigTableRowBase.h"
#include "Blueprint/BlueprintExceptionInfo.h"
#include "ConfigTableCache.generated.h"

/**
 * 表的Cache
 */
UCLASS()
class GAMEFRAMEWORK_API UConfigTableCache : public UBlueprintFunctionLibrary
{
public:
    GENERATED_UCLASS_BODY()

    /** clear cache */
    UFUNCTION(BlueprintCallable, Category = "DataTable")
    static void Clear();

    /** Get a Row from a DataTable given a id */
    UFUNCTION(BlueprintCallable, CustomThunk, Category = "DataTable", meta = (CustomStructureParam = "OutRow", BlueprintInternalUseOnly = "true"))
    static bool GetDataTableRowFromId(UDataTable* Table, int32 Id, FConfigTableRowBase& OutRow);

    /** Get all ids */
    UFUNCTION(BlueprintCallable, Category = "DataTable")
    static TArray<int32> GetDataTableIds(const UDataTable* DataTable);

    static FConfigTableRowBase* GetDataTableRawDataFromId(const UDataTable* Table, int32 Id);
    static bool Generic_GetDataTableRowFromId(const UDataTable* Table, int32 Id, void* OutRowPtr);

    static const TMap<int32, FConfigTableRowBase*>* GetAllRows(const UDataTable* Table);

    /** Based on UConfigTableCache::GetDataTableRowFromId */
    DECLARE_FUNCTION(execGetDataTableRowFromId) {
        P_GET_OBJECT(UDataTable, Table);
        P_GET_PROPERTY(FUInt32Property, Id);

        Stack.StepCompiledIn<FStructProperty>(NULL);
        void* OutRowPtr = Stack.MostRecentPropertyAddress;

        P_FINISH;
        bool bSuccess = false;

        FStructProperty* StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
        if (!Table) {
            FBlueprintExceptionInfo ExceptionInfo(
                EBlueprintExceptionType::AccessViolation,
                NSLOCTEXT("GetDataTableRow", "MissingTableInput", "Failed to resolve the table input. Be sure the DataTable is valid.")
            );
            FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
        }
        else if (StructProp && OutRowPtr) {
            UScriptStruct* OutputType = StructProp->Struct;
            const UScriptStruct* TableType = Table->GetRowStruct();

            const bool bCompatible = (OutputType == TableType) ||
                (OutputType->IsChildOf(TableType) && FStructUtils::TheSameLayout(OutputType, TableType));
            if (bCompatible) {
                P_NATIVE_BEGIN;
                bSuccess = Generic_GetDataTableRowFromId(Table, Id, OutRowPtr);
                P_NATIVE_END;
            }
            else {
                FBlueprintExceptionInfo ExceptionInfo(
                    EBlueprintExceptionType::AccessViolation,
                    NSLOCTEXT("GetDataTableRow", "IncompatibleProperty", "Incompatible output parameter; the data table's type is not the same as the return type.")
                );
                FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
            }
        }
        else {
            FBlueprintExceptionInfo ExceptionInfo(
                EBlueprintExceptionType::AccessViolation,
                NSLOCTEXT("GetDataTableRow", "MissingOutputProperty", "Failed to resolve the output parameter for GetDataTableRow.")
            );
            FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
        }
        *(bool*)RESULT_PARAM = bSuccess;
    }

    //写这个函数，要将缓存的表数据加上引用，避免gc
    static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

private:
    static TMap<int32, FConfigTableRowBase*>* FindAndInitTable(const UDataTable* Table);
};