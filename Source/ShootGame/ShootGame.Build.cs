// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShootGame : ModuleRules
{
	public ShootGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG",
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore"});

		DynamicallyLoadedModuleNames.AddRange(
				new string[] {
				"OnlineSubsystemNull"
				}
			);
	}
}
