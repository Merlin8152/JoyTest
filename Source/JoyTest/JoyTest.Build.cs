// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JoyTest : ModuleRules
{
	public JoyTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine", 
			"UMG",
			"Slate",
            "InputCore",
			"SlateCore",
			"InputCore",});
	}
}
