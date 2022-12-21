// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSUtilsModule.h"

#include "RWSUtilsDefines.h"
#include "RWSUtilsSettings.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FRWSUtilsModule"

const FName FRWSUtilsModule::ModuleName = "RWSUtils";

void FRWSUtilsModule::StartupModule()
{
	// Register settings
	RWSUtilsSettings = NewObject<URWSUtilsSettings>(GetTransientPackage(), "RWSUtilsSettings", RF_Standalone);
	RWSUtilsSettings->AddToRoot();

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", ModuleName,
			LOCTEXT("RuntimeSettingsName", "RWS Utils"),
			LOCTEXT("RuntimeSettingsDescription", "Configure RWS Utils"),
			RWSUtilsSettings);
	}
}

void FRWSUtilsModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", ModuleName);
	}

	if (!GExitPurge)
	{
		// If we're in exit purge, this object has already been destroyed
		RWSUtilsSettings->RemoveFromRoot();
	}
	else
	{
		ImageLoader = nullptr;
		RWSUtilsSettings = nullptr;
	}
}

URWSUtilsImageLoader* FRWSUtilsModule::GetImageLoader()
{
	return ImageLoader;
}

URWSUtilsSettings* FRWSUtilsModule::GetSettings() const
{
	check(RWSUtilsSettings);
	return RWSUtilsSettings;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRWSUtilsModule, RWSUtils);

DEFINE_LOG_CATEGORY(LogRWSUtils);