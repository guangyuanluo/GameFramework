#include "DataTableSeedConfigTableHelper.h"

FString DataTableSeedConfigTableHelper::GetDataTablePackagePath() {
	return TEXT(
		"/Game/Data/BaseConfigTable/"
	);
}

FString DataTableSeedConfigTableHelper::GetDataTableAssetName() {
	return TEXT(
		"DT_DataTableSeed"
	);
}

UDataTable* DataTableSeedConfigTableHelper::GetDataTable() {
	return Cast<UDataTable>(StaticLoadObject(
		UDataTable::StaticClass(), NULL, *FString::Format(TEXT("DataTable'{0}{1}.{2}'"), { GetDataTablePackagePath(), GetDataTableAssetName(), GetDataTableAssetName() })));
}