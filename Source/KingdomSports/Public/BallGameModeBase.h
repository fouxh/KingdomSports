// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BallGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KINGDOMSPORTS_API ABallGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ABallGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		TArray<APlayerController*> Controllers;
};
