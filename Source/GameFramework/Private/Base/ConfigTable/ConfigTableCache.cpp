// Fill out your copyright notice in the Description page of Project Settings.

#include "ConfigTableCache.h"
#include "Base/ConfigTable/ConfigTableCache.h"

TMap<FString, TMap<int32, FConfigTableRowBase*>> AllCacheRows;
TMap<FString, const UScriptStruct*> TableRowStruct;

UConfigTableCache::UConfigTableCache(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {
}

void UConfigTableCache::Clear() {
    for (auto TableIter = AllCacheRows.begin(); TableIter; ++TableIter) {
        auto FindTableRowScript = TableRowStruct.Find(TableIter->Key);
        if (FindTableRowScript) {
            for (auto RowIter = TableIter->Value.begin(); RowIter; ++RowIter) {
                auto RowData = RowIter->Value;
                (*FindTableRowScript)->DestroyStruct(RowData);
                FMemory::Free(RowData);
            }
        }
    }
    AllCacheRows.Empty();
    TableRowStruct.Empty();
}

bool UConfigTableCache::GetDataTableRowFromId(UDataTable* Table, int32 Id, FConfigTableRowBase& OutRow) {
    // We should never hit this!  stubs to avoid NoExport on the class.
    check(0);
    return false;
}

TArray<int32> UConfigTableCache::GetDataTableIds(const UDataTable* DataTable) {
    TArray<int32> Result;
    TArray<FConfigTableRowBase*> AllRows;
    DataTable->GetAllRows("", AllRows);

    for (int Index = 0; Index < AllRows.Num(); ++Index) {
        auto Row = AllRows[Index];
        Result.Add(Row->GetUniqueId());
    }

    return Result;
}

FConfigTableRowBase* UConfigTableCache::GetDataTableRawDataFromId(const UDataTable* Table, int32 Id) {
    auto FindTableIter = FindAndInitTable(Table);
    if (FindTableIter) {
        auto FindRowPtr = FindTableIter->Find(Id);
        if (FindRowPtr) {
            return *FindRowPtr;
        }
    }

    return nullptr;
}

const TMap<int32, FConfigTableRowBase*>* UConfigTableCache::GetAllRows(const UDataTable* Table) {
    auto FindTableIter = FindAndInitTable(Table);
    if (FindTableIter) {
        return FindTableIter;
    }
    
    return nullptr;
}

bool UConfigTableCache::Generic_GetDataTableRowFromId(const UDataTable* Table, int32 Id, void* OutRowPtr) {
    if (OutRowPtr && Table) {
        auto TableUsingStruct = Table->GetRowStruct();

        if (TableUsingStruct) {
            auto FindRawDataPtr = UConfigTableCache::GetDataTableRawDataFromId(Table, Id);

            if (FindRawDataPtr) {
                TableUsingStruct->CopyScriptStruct(OutRowPtr, FindRawDataPtr);

                return true;
            }
        }
    }
    
    return false;
}

TMap<int32, FConfigTableRowBase*>* UConfigTableCache::FindAndInitTable(const UDataTable* Table) {
    if (Table) {
        auto TableUsingStruct = Table->GetRowStruct();

        if (TableUsingStruct) {
            FString TablePathName = Table->GetPathName();
            auto FindTableIter = AllCacheRows.Find(TablePathName);
            if (!FindTableIter) {
                auto& RowMap = AllCacheRows.Add(TablePathName);

                TArray<FConfigTableRowBase*> AllRows;
                Table->GetAllRows("", AllRows);

                TableRowStruct.Add(TablePathName, TableUsingStruct);

                int32 StructureSize = TableUsingStruct->GetStructureSize();

                for (int Index = 0; Index < AllRows.Num(); ++Index) {
                    auto RowData = AllRows[Index];
                    FConfigTableRowBase* NewRow = (FConfigTableRowBase*)FMemory::Malloc(StructureSize);

                    TableUsingStruct->InitializeStruct(NewRow);
                    TableUsingStruct->CopyScriptStruct(NewRow, RowData);

                    RowMap.Add(NewRow->GetUniqueId(), NewRow);
                }

                FindTableIter = AllCacheRows.Find(TablePathName);
            }
            return FindTableIter;
        }
    }
    return nullptr;
}