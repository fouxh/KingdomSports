// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateBrush.h"
#include "RWSUtilsTheme.generated.h"

UCLASS(abstract, Blueprintable, BlueprintType)
class URWSUtilsTheme : public UObject
{
	GENERATED_BODY()

public:
	/** Get theme color with specified name. */
	UFUNCTION(BlueprintPure, Category = "RWS UI Theme")
	FColor GetColor(const FString& ColorName) const;
	
	/** Get theme font with specified name. */
	UFUNCTION(BlueprintPure, Category = "RWS UI Theme")
	FSlateFontInfo GetFont(const FString& FontName) const;
	
	/** Get theme brush with specified name. */
	UFUNCTION(BlueprintPure, Category = "RWS UI Theme")
	FSlateBrush GetBrush(const FString& BrushName) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RWS UI Theme")
	TMap<FString, FColor> Colors;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RWS UI Theme")
	TMap<FString, FSlateFontInfo> Fonts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RWS UI Theme")
	TMap<FString, FSlateBrush> Brushes;
};
