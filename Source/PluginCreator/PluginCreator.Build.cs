// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PluginCreator : ModuleRules
{
	public PluginCreator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
