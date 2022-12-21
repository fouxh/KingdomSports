// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"

#include "RWSUtilsWidgetsLibrary.generated.h"

UCLASS(abstract, Blueprintable, BlueprintType)
class URWSUtilsWidgetsLibrary : public UObject
{
	GENERATED_BODY()

public:
	
	/** Get theme brush with specified name. */
	UFUNCTION(BlueprintPure, Category = "RWS Widgets Library")
	TSubclassOf<UUserWidget> GetWidget(const FString& WidgetName) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RWS Widgets Library")
	TMap<FString, TSubclassOf<UUserWidget>> Widgets;
};
