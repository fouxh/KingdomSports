// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class RWSUtils : ModuleRules
    {
        public RWSUtils(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Slate",
                    "SlateCore",
                    "UMG",
                    "Engine",
                    "ImageWrapper",
                    "HTTP",
                    "Json",
                    "JsonUtilities",
                    "Projects",
                    "InputCore"
                }
            );

            PublicDefinitions.Add("WITH_RWS_UTILS=1");
        }
    }
}
