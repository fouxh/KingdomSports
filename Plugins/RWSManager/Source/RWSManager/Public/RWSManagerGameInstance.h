// Copyright 2021 ReverseWinterStudios Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "RWSManagerGameInstance.generated.h"

UCLASS()
class RWSMANAGER_API URWSManagerGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	FString Unique_Player_Matchmaking_ID;
};
