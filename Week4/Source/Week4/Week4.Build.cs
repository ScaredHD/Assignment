// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Week4 : ModuleRules
{
	public Week4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
