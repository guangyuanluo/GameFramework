// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class GAMEFRAMEWORK_API DataTableSeedConfigTableHelper {
public:
	/**
	* @brief 得到数据表格
	*/	
	static FString GetDataTablePackagePath();
	static FString GetDataTableAssetName();
	static class UDataTable* GetDataTable();
};