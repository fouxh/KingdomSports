// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGKCharacter.h"

AMyGKCharacter::AMyGKCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyGKCharacter::UpdateLocation_Implementation()
{

}

void AMyGKCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
}

void AMyGKCharacter::Tick(float DeltaTime)
{
	UpdateLocation();
}