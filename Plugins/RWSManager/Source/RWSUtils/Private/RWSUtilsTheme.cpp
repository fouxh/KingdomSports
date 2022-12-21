// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#include "RWSUtilsTheme.h"

FColor URWSUtilsTheme::GetColor(const FString& ColorName) const
{
	if (Colors.Contains(ColorName))
	{
		return Colors[ColorName];
	}

	return FColor();
}

FSlateFontInfo URWSUtilsTheme::GetFont(const FString& FontName) const
{
	if (Fonts.Contains(FontName))
	{
		return Fonts[FontName];
	}

	return FSlateFontInfo();
}

FSlateBrush URWSUtilsTheme::GetBrush(const FString& BrushName) const
{
	if (Brushes.Contains(BrushName))
	{
		return Brushes[BrushName];
	}

	return FSlateBrush();
}

