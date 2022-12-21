// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class RWSManager : ModuleRules
    {
        public RWSManager(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "WebBrowser",
                    "Slate",
                    "SlateCore",
                    "HTTP",
                    "Json",
                    "JsonUtilities",
                    "UMG",
                    "Engine",
                    "OnlineSubsystemUtils",
                    "RWSUtils"
                }
            );
			
			 PublicDefinitions.Add("WITH_RWS_MANAGER=1");
        }
    }
}