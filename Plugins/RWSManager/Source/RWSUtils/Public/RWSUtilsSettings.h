// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"

#include "RWSUtilsDataModel.h"
#include "RWSUtilsTheme.h"
#include "RWSUtilsWidgetsLibrary.h"

#include "RWSUtilsSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class RWSUTILS_API URWSUtilsSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	/** Interface theme. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Utils Settings")
	TSubclassOf<URWSUtilsTheme> InterfaceTheme;

	/* Widgets library. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Utils Settings")
	TSubclassOf<URWSUtilsWidgetsLibrary> WidgetsLibrary;
};
