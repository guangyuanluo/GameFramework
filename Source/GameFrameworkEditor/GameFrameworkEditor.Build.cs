// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class GameFrameworkEditor : ModuleRules
{
	public GameFrameworkEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "GameFramework",
				// ... add other public dependencies that you statically link with here ...
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
                "GraphEditor",
                "Kismet",
                "EditorStyle",
                "BlueprintGraph",
                "PropertyEditor",
                "Json",
                "JsonUtilities",
                "ToolMenus",
                "ApplicationCore",
            }
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
                "PlacementMode",
            }
			);

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "PlacementMode",
            }
        );

        // Enable C++ Exceptions for this module
        bEnableExceptions = true;

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true        
        PublicIncludePaths.Add(ModuleDirectory);
        PrivateIncludePaths.AddRange(new string[] {
            "GameFrameworkEditor/Public",
            "GameFrameworkEditor/Public/Condition",
            "GameFrameworkEditor/Private/Condition",
            "GameFrameworkEditor/Public/Reward",
            "GameFrameworkEditor/Private/Reward",
            "GameFrameworkEditor/Private",
            "GameFrameworkEditor/Private/Enum",            
            "GameFrameworkEditor/Private/Graph",
            "GameFrameworkEditor/Private/Scenario",
            "GameFrameworkEditor/Private/Skill",
            "GameFrameworkEditor/Private/Item",
            "GameFrameworkEditor/Private/Money",
            "GameFrameworkEditor/Private/Exp",
            "GameFrameworkEditor/Private/Store",
            "GameFrameworkEditor/Private/Unit",
            "GameFrameworkEditor/Private/Quest/QuestDetail",
            "GameFrameworkEditor/Private/Quest/QuestTree",
            "GameFrameworkEditor/Public/Slate",
            "GameFrameworkEditor/Private/Placement",
            "GameFrameworkEditor/Public/Scenario",
        });
    }

    private string GameFrameworkPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../GameFramework/")); }
    }
}
