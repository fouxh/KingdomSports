// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSUtilsSettings.h"

#include "UObject/ConstructorHelpers.h"
#include "RWSUtilsLibrary.h"
#include "RWSUtilsModule.h"

URWSUtilsSettings::URWSUtilsSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<URWSUtilsTheme> ThemeFinder(*FString::Printf(TEXT("/%s/Utils/BP_DefaultTheme.BP_DefaultTheme_C"),
		*URWSUtilsLibrary::GetPluginName(FRWSUtilsModule::ModuleName)));
	static ConstructorHelpers::FClassFinder<URWSUtilsWidgetsLibrary> WidgetsLibraryFinder(*FString::Printf(TEXT("/%s/Utils/BP_DefaultWidgetsLibrary.BP_DefaultWidgetsLibrary_C"),
		*URWSUtilsLibrary::GetPluginName(FRWSUtilsModule::ModuleName)));
	InterfaceTheme = ThemeFinder.Class;
	WidgetsLibrary = WidgetsLibraryFinder.Class;
}
