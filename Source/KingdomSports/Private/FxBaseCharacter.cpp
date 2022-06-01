// Fill out your copyright notice in the Description page of Project Settings.


#include "FxBaseCharacter.h"
#include "GoalActor.h"
#include "BallGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AFxBaseCharacter::AFxBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFxBaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFxBaseCharacter, PlayerBallStatus);
	DOREPLIFETIME(AFxBaseCharacter, BallRefFx);
	DOREPLIFETIME(AFxBaseCharacter, IsStunned);
	DOREPLIFETIME(AFxBaseCharacter, Target);
}

EIntentDirectionTypeFx AFxBaseCharacter::GetDirection()
{
	float Fwd_loc=MoveForwardFx;
	float Right_loc=MoveRightFx;
	bool _is_right;
	bool _is_left;
	bool _is_up;
	bool _is_down;
	bool _is_upRight;
	bool _is_upLeft;
	bool _is_downRight;
	bool _is_downleft;
	_is_right=Right_loc >0.1;
	_is_left=Right_loc<0.0;
	_is_up=Fwd_loc >0.1;
	_is_down=Fwd_loc < 0.0;
	_is_upRight=_is_up && _is_right;
	_is_upLeft=_is_up && _is_left;
	_is_downRight=_is_down && _is_right;
	_is_downleft=_is_down && _is_left;
	if(_is_upRight)
	{
		DirectionIntent=EIntentDirectionTypeFx::UPRight;
	}
	else
	{
		if(_is_downRight)
		{
			DirectionIntent=EIntentDirectionTypeFx::DownRight;
		}
		else
		{
			if(_is_upLeft)
			{
				DirectionIntent=EIntentDirectionTypeFx::UPLeft;
			}
			else
			{
				if(_is_downleft)
				{
					DirectionIntent=EIntentDirectionTypeFx::DownLeft;
				}
				else
				{
					if(_is_left)
					{
						DirectionIntent=EIntentDirectionTypeFx::Left;
					}
					else
					{
						if(_is_right)
						{
							DirectionIntent=EIntentDirectionTypeFx::Right;
						}
						else
						{
							if(_is_up)
							{
								DirectionIntent=EIntentDirectionTypeFx::Up;
							}
							else
							{
								if(_is_down)
								{
									DirectionIntent=EIntentDirectionTypeFx::Down;
								}
								else
								{
									DirectionIntent=EIntentDirectionTypeFx::None;
								}
							}
						}
					}

				}
			}
		}
	}
	return DirectionIntent;
}

void AFxBaseCharacter::InitCharacter()
{

}

//void AFxBaseCharacter::StartMoveWithTheBall()
//{
//	if(PlayerBallStatus==EPlayerBallStatusFx::EPossessBall && BallRefFx)
//	{
//		FVector NewLocation = (GetActorForwardVector() * MaxBallDistance) + (GetActorRightVector() * 10.f) + GetActorLocation();
//		NewLocation.Z = 16;
//		MoveBall(NewLocation);
//	/*
//	float Speed = GetVelocity().Size();
//	FHitResult HitResultFx;
//	FVector Start = (GetActorForwardVector()*MaxBallDistance)+GetActorLocation();
//	FVector End = Start+FVector(0.0,0.0,-500);
//	GetWorld()->LineTraceSingleByChannel(HitResultFx, Start, End, ECollisionChannel::ECC_Visibility);
//	if (HitResultFx.bBlockingHit)
//		MoveBall(HitResultFx.Location);*/
//	}
//}
//
//void AFxBaseCharacter::MoveBall(FVector Location)
//{
//	if (GetNetMode() == ENetMode::NM_Standalone)
//		BallMovement(Location);
//
//	if (HasAuthority())
//		return;
//	else
//	{
//		MoveBall_Client(Location);
//		MoveBall_Srv(Location);
//	}
//}
//
//void AFxBaseCharacter::BallMovement(FVector Location)
//{
//	if (BallRefFx)
//	{
//		FVector MoveLocation = UKismetMathLibrary::VLerp(BallRefFx->GetActorLocation(), Location, 0.1f);
//		float DistanceBall = UKismetMathLibrary::Vector_Distance(BallRefFx->GetActorLocation(), Location);
//		float RotateBallSpeed;
//		if (DistanceBall <= 2.0)
//			RotateBallSpeed = 0.001;
//		else
//			RotateBallSpeed = 5;
//		FRotator TargetRotation = BallRefFx->GetActorRotation();
//		TargetRotation.Roll += RotateBallSpeed;
//		BallRefFx->SetActorLocationAndRotation(MoveLocation, TargetRotation);
//	}
//}
//
//void AFxBaseCharacter::MoveBall_Srv_Implementation(FVector Location)
//{
//	MoveBall_Multi(Location);
//}
//
//void AFxBaseCharacter::MoveBall_Multi_Implementation(FVector Location)
//{
//	if (IsLocallyControlled())
//		return;
//	else
//		BallMovement(Location);
//}
//
//void AFxBaseCharacter::MoveBall_Client_Implementation(FVector Location)
//{
//	BallMovement(Location);
//}

void AFxBaseCharacter::ResetBallPhysics_Implementation()
{
	BallRefFx->DisablePhysics_srv();
}

void AFxBaseCharacter::UpdateBallStatus_Implementation(EPlayerBallStatusFx _sts)
{
	PlayerBallStatus=_sts;
	OnBallStatusChange.Broadcast(_sts);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, FString::Printf(TEXT("Status Changed: %s"), *GetActorLabel()));
}

void AFxBaseCharacter::SetBallRef_Implementation(AFxBallActor* Ball_loc)
{
	BallRefFx=Ball_loc;
	BallRefFx->CharacterOwner=this;
	BallRefFx->RedOwned = ActorHasTag("Red");
}

void AFxBaseCharacter::GoalTarget(float DeltaTime)
{
	FHitResult Hit;
	FVector Start = GetActorLocation();
	FRotator Rotation = GetControlRotation();
	FVector End = Start + (UKismetMathLibrary::GetForwardVector(Rotation) * 5500.f);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility);

	if (Hit.bBlockingHit)
	{
		AGoalActor* GoalRef = Cast<AGoalActor>(Hit.GetActor());
		if (GoalRef)
		{
			if ((!GoalRef->RedNet && IsRed)||(!IsRed && GoalRef->RedNet))
			{
				GoalTargetLocation = Hit.Location;
				UChildActorComponent* GoalTarget = GoalRef->FindComponentByClass<UChildActorComponent>();
				if (GoalTarget)
					GoalTarget->SetWorldLocation(GoalTargetLocation);
			}
		}
	}
}

void AFxBaseCharacter::UpdateTick_Implementation(bool _Enable)
{
	SetActorTickEnabled(_Enable);
}

//void AFxBaseCharacter::PlayMontage_Srv(UAnimMontage* Animation)
//{
//	PlayMontage_Multi(Animation);
//}
//
//void AFxBaseCharacter::PlayMontage_Multi(UAnimMontage* Animation)
//{
//	PlayAnimMontage(Animation);
//}

void AFxBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitCharacter();
}

void AFxBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(PlayerBallStatus == EPlayerBallStatusFx::EPossessBall)
		GoalTarget(DeltaTime);
}

void AFxBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
