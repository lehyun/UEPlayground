// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleVariable.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "MyCustomSettings.h"

// LOCTEXT
#define LOCTEXT_NAMESPACE "CustomSettings"

class FConsoleVariableGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		RegisterSettings();
	}

	virtual void ShutdownModule() override
	{
		if (UObjectInitialized())
		{
			UnregisterSettings();
		}
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

private:

	// Callback for when the settings were saved.
	bool HandleSettingsSaved()
	{
		UMyCustomSettings* Settings = GetMutableDefault<UMyCustomSettings>();
		bool ResaveSettings = false;

		// You can put any validation code in here and resave the settings in case an invalid
		// value has been entered

		if (ResaveSettings)
		{
			Settings->SaveConfig();
		}

		return true;
	}

	void RegisterSettings()
	{
		// Registering some settings is just a matter of exposing the default UObject of
		// your desired class, feel free to add here all those settings you want to expose
		// to your LDs or artists.

		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			// Create the new category
			ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

			SettingsContainer->DescribeCategory("CustomSettings",
				LOCTEXT("RuntimeWDCategoryName", "CustomSettings"),
				LOCTEXT("RuntimeWDCategoryDescription", "Game configuration for the CustomSettings game module"));

			// Register the settings
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "CustomSettings", "General",
				LOCTEXT("RuntimeGeneralSettingsName", "General"),
				LOCTEXT("RuntimeGeneralSettingsDescription", "Base configuration for our game module"),
				GetMutableDefault<UMyCustomSettings>()
			);

			// Register the save handler to your settings, you might want to use it to
			// validate those or just act to settings changes.
			if (SettingsSection.IsValid())
			{
				SettingsSection->OnModified().BindRaw(this, &FConsoleVariableGameModule::HandleSettingsSaved);
			}
		}
	}

	void UnregisterSettings()
	{
		// Ensure to unregister all of your registered settings here, hot-reload would
		// otherwise yield unexpected results.

		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "CustomSettings", "General");
		}
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FConsoleVariableGameModule, ConsoleVariable, "ConsoleVariable" );
