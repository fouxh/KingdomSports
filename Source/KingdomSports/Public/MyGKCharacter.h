// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FxBaseCharacter.h"
#include "MyGKCharacter.generated.h"

UCLASS()
class KINGDOMSPORTS_API AMyGKCharacter : public AFxBaseCharacter
{
	GENERATED_BODY()
public:

	AMyGKCharacter();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetFocus(AActor* Actor);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateLocation();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
};