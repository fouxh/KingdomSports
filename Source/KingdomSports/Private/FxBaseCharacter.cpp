// Fill out your copyright notice in the Description page of Project Settings.


#include "FxBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AFxBaseCharacter::AFxBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	TArray<UActorComponent*> ArrowsRef=GetComponentsByTag(UActorComponent::StaticClass(),"Arrow");
	ArrowDirection=Cast<UArrowComponent>(ArrowsRef[0]);

}

void AFxBaseCharacter::HaveBallFx(AFxBallActor* Ball_loc)
{
	if(Ball_loc && PlayerBallStatus==EPlayerBallStatusFx::ENone)
	{
		SetBallRef(Ball_loc);
		UpdateBallStatus(EPlayerBallStatusFx::EPossessBall);
		OnBallStatusChange.Broadcast(EPlayerBallStatusFx::EPossessBall);
		 TArray < AActor * > PlayerArray_Out;
		 AFxBaseCharacter * Player_locref;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),GetClass(),PlayerArray_Out);
		for(int32 Index = 0; Index < PlayerArray_Out.Num(); ++Index)
		{
		Player_locref=Cast<AFxBaseCharacter>(PlayerArray_Out[Index]);
			if(Player_locref!=this)
			{
				Player_locref->UpdateBallStatus(EPlayerBallStatusFx::ENone);
				Player_locref->OnBallStatusChange.Broadcast(EPlayerBallStatusFx::ENone);
			}
		}
		//PlayerArray_loc

	}

}

void AFxBaseCharacter::StartMoveWithTheBall()
{
	if(PlayerBallStatus==EPlayerBallStatusFx::EPossessBall)
	{

	UWorld* world = GetWorld();
float Speed_loc=GetVelocity().Size();
//float Distanceloc=UKismetMathLibrary::MapRangeClamped(Speed_loc,0.0,600,MinBallDistance,MaxBallDistance);
	FVector StartLoc =(ArrowDirection->GetForwardVector()*MaxBallDistance)+GetActorLocation();
	FVector TargetLoc =StartLoc+FVector(0.0,0.0,-1000);
	TArray<AActor*> IngoreActors;
	IngoreActors.Add(this);
	FCollisionQueryParams TraceParams(TEXT("LineOfSight_Trace"), false, this);
	TArray<FHitResult> HitRetArray;
	FHitResult HitResultFx;
	bool isHit =world->LineTraceSingleByChannel(HitResultFx,StartLoc,TargetLoc, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
	if (isHit)
	{


		MovingWithBall_srv(HitResultFx.Location);




	}
	}
}

void AFxBaseCharacter::MovingWithBall_srv_Implementation(FVector _Loc)
{
	if(BallRefFx)
	{
		FVector _LocWithOffset=_Loc+FVector(0.0,0.0,15);
	//	BallRefFx->SetTargetLocation_internalFx(_LocWithOffset,true);
		BallRefFx->SetTargetLocationFx(_LocWithOffset,true);
	}
}

void AFxBaseCharacter::UpdateBallStatus_Implementation(EPlayerBallStatusFx _sts)
{
	PlayerBallStatus=_sts;

}

void AFxBaseCharacter::SetBallRef_Implementation(AFxBallActor* Ball_loc)
{

	BallRefFx=Ball_loc;
	BallRefFx->CharacterOwner=this;
}

// Called when the game starts or when spawned
void AFxBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitCharacter();


}

// Called every frame
void AFxBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFxBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

