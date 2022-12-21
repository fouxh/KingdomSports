// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "RWSManagerSettings.generated.h"


UCLASS(config = Engine, defaultconfig)
class RWSMANAGER_API URWSManagerSettings : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Project ID from your Developer Account. Required. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Manager Settings")
	FString ProjectID;

	/** Login ID from your Developer Account. Required. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Manager Settings")
	FString LoginID;

	/** MatchmakingDelay is ever amount of seconds it will retry. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Manager Settings | Matchmaking", meta = (ClampMin = "5", ClampMax = "50", UIMin = "5", UIMax = "50"))
	int MatchmakingDelay = 5;

	/** MatchmakingRetries amount of retries if no server found it will create another one. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Manager Settings | Matchmaking", meta = (ClampMin = "3", ClampMax = "25", UIMin = "3", UIMax = "25"))
	int MatchmakingRetries = 3;

	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Manager Settings | Matchmaking")
	FString MatchmakingSearchingMessage = "Matchmaking In Progress..";
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Manager Settings | Matchmaking")
	FString MatchmakingJoiningMessage = "Joining Session..";
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "RWS Manager Settings | Matchmaking")
	FString MatchmakingErrorMessage = "Failed To Find Session..";
};
