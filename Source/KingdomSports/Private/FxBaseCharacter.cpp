// Fill out your copyright notice in the Description page of Project Settings.


#include "FxBaseCharacter.h"
#include "GoalActor.h"
#include "BallGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/ArrowComponent.h"
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

void AFxBaseCharacter::StartMoveWithTheBall()
{
	if(PlayerBallStatus==EPlayerBallStatusFx::EPossessBall && BallRefFx)
	{
	float Speed = GetVelocity().Size();
	FHitResult HitResultFx;
	FVector Start = (GetActorForwardVector()*MaxBallDistance)+GetActorLocation();
	FVector End = Start+FVector(0.0,0.0,-500);
	GetWorld()->LineTraceSingleByChannel(HitResultFx, Start, End, ECollisionChannel::ECC_Visibility);
	if (HitResultFx.bBlockingHit)
		MovingWithBall_srv(HitResultFx.Location);
	}
}

void AFxBaseCharacter::ResetBallPhysics()
{
	BallRefFx->UpdatePhysics_srv(false);
}

void AFxBaseCharacter::MovingWithBall_srv_Implementation(FVector _Loc)
{
	FVector Offset = _Loc + FVector(0.0,0.0,16);
	BallRefFx->SetTargetLocationFx(Offset,true);
}

void AFxBaseCharacter::UpdateBallStatus_Implementation(EPlayerBallStatusFx _sts)
{
	PlayerBallStatus=_sts;
	OnBallStatusChange.Broadcast(_sts);
}

void AFxBaseCharacter::SetBallRef_Implementation(AFxBallActor* Ball_loc)
{
	BallRefFx=Ball_loc;
	BallRefFx->CharacterOwner=this;
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
			GoalTargetLocation = Hit.Location;
			UChildActorComponent* GoalTarget = GoalRef->FindComponentByClass<UChildActorComponent>();
			if (GoalTarget)
				GoalTarget->SetWorldLocation(GoalTargetLocation);
		}
	}
}

void AFxBaseCharacter::UpdateTick_Implementation(bool _Enable)
{
	SetActorTickEnabled(_Enable);
}

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
