// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSUtilsWidgetsLibrary.h"

TSubclassOf<UUserWidget> URWSUtilsWidgetsLibrary::GetWidget(const FString& WidgetName) const
{
	if (Widgets.Contains(WidgetName))
	{
		return Widgets[WidgetName];
	}

	return nullptr;
}
