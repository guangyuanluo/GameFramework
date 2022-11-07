// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GameFramework.h"
#include "Misc/NetworkVersion.h"

#define LOCTEXT_NAMESPACE "FGameFrameworkModule"

void FGameFrameworkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    FNetworkVersion::GetLocalNetworkVersionOverride.BindLambda([]() {
        auto engineVersion = FEngineVersion::Current();
        int major = engineVersion.GetMajor();
        int minor = engineVersion.GetMinor();
        int patch = engineVersion.GetPatch();
        FString VersionString = FString::Printf(TEXT("%s %d %d %d"),
            FApp::GetProjectName(),
            major,
            minor,
            patch
        );

        uint32 checksum = FCrc::StrCrc32(*VersionString.ToLower());
        UE_LOG(LogNetVersion, Log, TEXT("GameFramework Override Network Version:%s (Checksum: %u)"), *VersionString, checksum);
        return checksum;
    });
}

void FGameFrameworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameFrameworkModule, GameFramework)