// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Shadows : ModuleRules
{
	public Shadows(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "RHI", "RenderCore", "ShaderCore", "ProceduralMeshComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent", "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");
		if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		{
			if (UEBuildConfiguration.bCompileSteamOSS == true)
			{
				DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
			}
		}
	}
}
