// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class KingdomSports : ModuleRules
{
	public KingdomSports(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableUndefinedIdentifierWarnings = false;
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"GameplayAbilities", "GameplayTags", "GameplayTasks", "Core", "CoreUObject", "Engine", "InputCore","UMG","AIModule","ApexDestruction","TargetSystem","Networking", "Sockets"
				// ... add other public dependencies that you statically link with here ...
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore","NavigationSystem" ,"AdvancedSessions","AdvancedSteamSessions","TargetSystem"
				// ... add private dependencies that you statically link with here ...
			}
		);

		//PublicIncludePaths.AddRange(new string[] { "TargetSystem/Public", "TargetSystem/Classes", "TargetSystem/Private" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
