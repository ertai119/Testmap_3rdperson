// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Testmap_3rdperson : ModuleRules
{
	public Testmap_3rdperson(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
