// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FXData.h"
#include "GameFramework/Character.h"
#include "FxBaseCharacter.generated.h"

UCLASS()
class KINGDOMSPORTS_API AFxBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFxBaseCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx_new)
    EIntentDirectionTypeFx DirectionIntent;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
    float MoveForwardFx;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
    float MoveRightFx;
	UFUNCTION(BlueprintPure,Category=FX)
    EIntentDirectionTypeFx GetDirection();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
