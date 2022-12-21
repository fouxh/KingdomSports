// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSManager.h"
#include "RWSManagerDefines.h"
#include "RWSManagerSettings.h"
 
#include "Developer/Settings/Public/ISettingsModule.h"

DEFINE_LOG_CATEGORY(LogRWSManager);

#define LOCTEXT_NAMESPACE "FRWSManager"

void FRWSManager::StartupModule()
{
	RWSManagerSettings = NewObject<URWSManagerSettings>(GetTransientPackage(), "RWSManagerSettings", RF_Standalone);
	RWSManagerSettings->AddToRoot();

	// Register settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "RWSManager",
			LOCTEXT("RuntimeSettingsName", "RWS Manager"),
			LOCTEXT("RuntimeSettingsDescription", "Configure RWS Manager"),
			RWSManagerSettings);
	}

	UE_LOG(LogRWSManager, Log, TEXT("%s: RWSManager module started"), *VA_FUNC_LINE);
}
 
void FRWSManager::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "RWSManager");
	}

	if (!GExitPurge)
	{
		// If we're in exit purge, this object has already been destroyed
		RWSManagerSettings->RemoveFromRoot();
	}
	else
	{
		RWSManagerSettings = nullptr;
	}
}

URWSManagerSettings* FRWSManager::GetSettings() const
{
	check(RWSManagerSettings);
	return RWSManagerSettings;
}

#undef LOCTEXT_NAMESPACE
 
IMPLEMENT_MODULE(FRWSManager,RWSManager)