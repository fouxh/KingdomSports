// Fill out your copyright notice in the Description page of Project Settings.


#include "FxBallActor.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "MyGKCharacter.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "FxBaseCharacter.h"

// Sets default values
AFxBallActor::AFxBallActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->SetupAttachment(GetRootComponent());
	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollision"));
	BallCollision->SetupAttachment(Root);
	BallMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(BallCollision);
	SplineComponentFx= CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	StartLocation =TargetLocation=GetActorLocation();
	
}

void AFxBallActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFxBallActor, IsBallCurve);
	
	//DOREPLIFETIME(AFxBallActor, CharacterOwner);
}

void AFxBallActor::InitBall()
{
	if(CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this,FName("TimelineProgress"));
		CurveFTimeline.AddInterpFloat(CurveFloat,TimelineProgress);
		OnFinishedTimeline.BindUFunction(this, FName("FinishMoveBallCurve"));
		CurveFTimeline.SetTimelineFinishedFunc(OnFinishedTimeline);
	}
	BallCollision->OnComponentBeginOverlap.AddDynamic(this, &AFxBallActor::OnOverlapActivateSphere);
	Gamestate = UGameplayStatics::GetGameState(GetWorld());
}

void AFxBallActor::FinishMoveBallCurve()
{
	IsBallCurve=false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Reach"));
}

void AFxBallActor::TimelineProgress(float _value)
{
	if(GetSplineComponent())
	{
		FVector NewLocaction=GetSplineComponent()->GetLocationAtTime(_value,ESplineCoordinateSpace::Local,false);
		FRotator TargetRotation=GetActorRotation();
		TargetRotation.Pitch += RotateBallSpeed;
		SetActorLocation(NewLocaction);
		if (CharacterOwner && CharacterOwner->MoveForwardFx > 0.2)
			SetActorRotation(TargetRotation);
		DistanceBall=UKismetMathLibrary::Vector_Distance(GetActorLocation(),TargetLocation);
	}
}

void AFxBallActor::Moveball_Curve(FVector Target_loc, float offsetZ, float offsetX)
{
	if(GetSplineComponent() && !IsBallCurve)
	{
		IsBallCurve=true;
		StartLocation=GetActorLocation();
		TargetLocation=Target_loc;
		FVector locVector=(StartLocation+TargetLocation)/2;
		FVector Middle_Vector=locVector+FVector(offsetX,-1,offsetZ);
		TArray<FVector> PointLoc;
		PointLoc.Add(StartLocation);
		PointLoc.Add(Middle_Vector);
		PointLoc.Add(TargetLocation);
		GetSplineComponent()->SetSplinePoints(PointLoc,ESplineCoordinateSpace::Local,true);
		CurveFTimeline.PlayFromStart();
	}
}

void AFxBallActor::DisablePhysics_srv_Implementation()
{
	DisablePhysics_Multi();
}

void AFxBallActor::DisablePhysics_Multi_Implementation()
{
	SetActorLocation(BallCollision->GetComponentLocation());
	BallCollision->SetSimulatePhysics(false);
	BallCollision->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AFxBallActor::OnOverlapActivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AFxBaseCharacter* CharLoc = Cast<AFxBaseCharacter>(OtherActor);
	if (CharLoc) 
	{
		if ((CharLoc->PlayerBallStatus == EPlayerBallStatusFx::EPossessBall && CharLoc == CharacterOwner) || CharLoc->IsStunned)
			return;

		if (!CharLoc->IsStunned)
		{
			CharLoc->SetBallRef(this);
			CharLoc->UpdateBallStatus(EPlayerBallStatusFx::EPossessBall);
			CharLoc->UpdateTick(true);
			CharLoc->SetActorTickEnabled(true);
			for (int i = 0; i < Gamestate->PlayerArray.Num(); ++i)
			{
				AFxBaseCharacter* Player_locref = Cast<AFxBaseCharacter>(Gamestate->PlayerArray[i]->GetPawn());
				if (Player_locref)
				{
					if (Player_locref != CharLoc)
					{
						if (AMyGKCharacter* GK_LocRef = Cast<AMyGKCharacter>(Player_locref))
						{
							GK_LocRef->UpdateBallStatus(EPlayerBallStatusFx::ENone);
							continue;
						}
						Player_locref->UpdateTick(false);
						Player_locref->SetActorTickEnabled(false);
						Player_locref->UpdateBallStatus(EPlayerBallStatusFx::ENone);
					}
				}
			}
		}

		AMyGKCharacter* GK = Cast<AMyGKCharacter>(CharacterOwner);
		if (GK)
		{
			DisablePhysics_srv();
			AttachToComponent(GK->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), "BallHand");
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("GK"));
			return;
		}

		if (BallCollision->IsSimulatingPhysics())
			CharLoc->ResetBallPhysics();
	}
}

void AFxBallActor::BeginPlay()
{
	Super::BeginPlay();
	InitBall();
}

void AFxBallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		if (IsBallCurve)
			CurveFTimeline.TickTimeline(DeltaTime);		
}