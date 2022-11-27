// Some copyright should be here...

using UnrealBuildTool;
using System.IO;
using System;

public class GameFramework : ModuleRules
{
	public GameFramework(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add other public dependencies that you statically link with here ...
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "UMG",
                "Json",
                "JsonUtilities",
                "AIModule",
                "GameplayAbilities",
                "GameplayTasks",
                "GameplayTags",
            }
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
                "Slate",
                "SlateCore",
                "Sockets",
                "DeveloperSettings"
            }
			);


        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
            }
			);

        if (Target.bBuildEditor == true) {
            PrivateDependencyModuleNames.AddRange(new string[] {
                "UnrealEd",
                "KismetCompiler",
                "BlueprintGraph",
            });
        }
        PublicIncludePaths.Add(ModuleDirectory);
        PrivateIncludePaths.AddRange(new string[] {
			//Frame
            "GameFramework/Public/Character",
            "GameFramework/Public/Base/ECS",
            "GameFramework/Public/Base/Log",
            "GameFramework/Public/Base/Subsystem",
            "GameFramework/Public/Base/ConfigTable",
            "GameFramework/Public/Base/Structure",
            "GameFramework/Public/GameInstance",
            "GameFramework/Public/GameMode",
            "GameFramework/Public/PlayerController",
			"GameFramework/Public/PlayerState",
            //Data
            "GameFramework/Public/Data",
            "GameFramework/Public/Data/Area",            
            "GameFramework/Public/Data/Hero",
            "GameFramework/Public/Data/Team",
            //Editor
            "GameFramework/Public/Editor/Enum",
            "GameFramework/Public/Editor/Table",
            //Modules
            "GameFramework/Public/Modules/Events",
            "GameFramework/Public/Modules/Assets",
            "GameFramework/Public/Modules/Condition",
            "GameFramework/Private/Modules/Condition",
            "GameFramework/Public/Modules/Exp",
            "GameFramework/Public/Modules/Player",
            "GameFramework/Public/Modules/Quest",
            "GameFramework/Public/Modules/Item",
            "GameFramework/Public/Modules/Team",
            "GameFramework/Public/Modules/Skill",
            "GameFramework/Public/Modules/GM",
            "GameFramework/Public/Modules/Money",
            "GameFramework/Public/Modules/Unit",
            "GameFramework/Public/Modules/Skill",
            "GameFramework/Public/Modules/Store",
            "GameFramework/Public/Modules/Order",
            "GameFramework/Public/Modules/Reward",
            //Global
            "GameFramework/Public/Global/Events",
            "GameFramework/Public/Global/Conditions",
            "GameFramework/Public/Global/Orders",
            "GameFramework/Public/Global/Rewards",
            //Other
            "GameFramework/Public/UI",
            "GameFramework/Public/Utils",
            "GameFramework/Public/Utils/Async",
            "GameFramework/Public/Utils/Algorithm",
            "GameFramework/Public/Utils/Item",
        });
        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
