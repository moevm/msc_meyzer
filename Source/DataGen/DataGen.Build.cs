// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class DataGen : ModuleRules
{
	public DataGen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" });
		string ProjectPath = Path.Combine(ModuleDirectory, "..", "..");
		string ThirdPartyPath = Path.Combine(ProjectPath, "ThirdParty");

		if (Target.Platform == UnrealTargetPlatform.Win64) {
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "assimp-4.1.0", "lib", "Win64", "assimp-vc140-mt.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "assimp-4.1.0", "lib", "Win64", "IrrXML.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "assimp-4.1.0", "lib", "Win64", "zlibstatic.lib"));
			PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "assimp-4.1.0", "include"));

			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "tinyfiledialogs", "lib", "Win64", "tinyfiledialogs.lib"));
			PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "tinyfiledialogs", "include"));
		}
		else {
			throw new PlatformNotSupportedException("Build for your platform is not supported yet. You can add support for it in DataGen.Build.cs");
		}

		// Add any include paths for the plugin
		// PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Assimp", "include"));

		// if (Target.Platform == UnrealTargetPlatform.Win64) {
		//     PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Win64", "assimp-vc140-mt.lib"));
		//     PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Win64", "IrrXML.lib"));
		//     PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Win64", "zlibstatic.lib"));
		// } else {
		//     throw new PlatformNotSupportedException("Build for your platform is not supported yet. You can add support for it in Assimp.build.cs");
		// }
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
