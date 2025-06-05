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
                "NetCore",
                "UMG",
                "Json",
                "JsonUtilities",
                "AIModule",
                "GameplayAbilities",
                "GameplayTasks",
                "GameplayTags",
                "EnhancedInput",
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
        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
