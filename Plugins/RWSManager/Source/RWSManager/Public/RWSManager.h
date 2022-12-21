// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once
 
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class URWSManagerSettings;
/**
 * RWSManager SDK Module
 * See: https://developers.reversewinterstudios.com/api/v1/documentation/server-manager/
 */
class FRWSManager : public IModuleInterface
{
public:
 
	/* This will get called when the editor loads the module */
	virtual void StartupModule() override;
 
	/* This will get called when the editor unloads the module */
	virtual void ShutdownModule() override;

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase.  Your module might be unloaded already.
	 *
	 * @return Returns a singleton instance, loading the module on demand if needed.
	 */
	static inline FRWSManager& Get()
	{
		return FModuleManager::LoadModuleChecked<FRWSManager>("RWSManager");
	}

	/** Getter for internal settings object to support runtime configuration changes. */
	URWSManagerSettings* GetSettings() const;

private:
	/** Module settings */
	URWSManagerSettings* RWSManagerSettings;
};