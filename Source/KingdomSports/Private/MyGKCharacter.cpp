// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGKCharacter.h"
#include "AIController.h"


AMyGKCharacter::AMyGKCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyGKCharacter::UpdateLocation_Implementation()
{

}

void AMyGKCharacter::SetFocus_Implementation(AActor* Actor)
{
	AAIController* GKController = Cast<AAIController>(GetController());
	GKController->SetFocus(Actor);
}

void AMyGKCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyGKCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLocation();
}