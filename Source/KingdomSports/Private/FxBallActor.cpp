// Fill out your copyright notice in the Description page of Project Settings.


#include "FxBallActor.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "FxBaseCharacter.h"

// Sets default values
AFxBallActor::AFxBallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	BallMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(GetRootComponent());
	SplineComponentFx= CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	StartLocation =TargetLocation=GetActorLocation();
	BallCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BallCollision"));
	BallCollision->SetupAttachment(BallMesh);
}
void AFxBallActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(AFxBallActor, IsBallCurve);
	/*DOREPLIFETIME(AFxBallActor, IsBallMove);
	DOREPLIFETIME(AFxBallActor, StartLocation);
	DOREPLIFETIME(AFxBallActor, TargetLocation);
	*/


}


void AFxBallActor::UpdateBallLocation_srv_Implementation()
{
	if(IsBallMove)
	{
		FVector MoveLocation_loc=UKismetMathLibrary::VLerp(StartLocation,TargetLocation,BallSpeed);
		FRotator TargetRotaion=FRotator(GetActorRotation().Pitch,GetActorRotation().Yaw,GetActorRotation().Roll+RotateBallSpeed);

		SetActorLocation(MoveLocation_loc);
		if(CharacterOwner)
		{
			if(CharacterOwner->MoveForwardFx > 0.2)
			{
				SetActorRotation(TargetRotaion);
			}

		}

	}

}

void AFxBallActor::SetTargetLocationFx_Implementation(FVector TargetLocation_loc, bool _Enable)
{
	StartLocation=GetActorLocation();
	TargetLocation=TargetLocation_loc;
	IsBallMove=true;
}

void AFxBallActor::UpdateBallLocation()
{
	if(IsBallMove)
	{
		FVector MoveLocation_loc=UKismetMathLibrary::VLerp(StartLocation,TargetLocation,BallSpeed);
		//RotateBallSpeed=UKismetMathLibrary::lerp()
		DistanceBall=UKismetMathLibrary::Vector_Distance(GetActorLocation(),TargetLocation);
		if(DistanceBall<=2.0)
		{
			RotateBallSpeed=0.001;
		}
		else
		{
			RotateBallSpeed=5;
		}
		FRotator TargetRotaion=FRotator(GetActorRotation().Pitch,GetActorRotation().Yaw,GetActorRotation().Roll+RotateBallSpeed);

			SetActorLocation(MoveLocation_loc);


		SetActorRotation(TargetRotaion);


	}
}

void AFxBallActor::SetTargetLocation_internalFx(FVector TargetLocation_loc, bool _Enable)
{
	StartLocation=GetActorLocation();
	TargetLocation=TargetLocation_loc;
	IsBallMove=true;
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
		//CurveFTimeline.SetLooping(true);

		//CurveFTimeline.PlayFromStart();

	}
	BallCollision->OnComponentBeginOverlap.AddDynamic(this, &AFxBallActor::OnOverlapActivateSphere);
}

void AFxBallActor::FinishMoveBallCurve()
{
	IsBallCurve=false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Reach")));
}

void AFxBallActor::TimelineProgress(float _value)
{
	if(GetSplineComponent())
	{
		//FVector NewLocaction=FMath::Lerp(StartLocation,TargetLocation,_value);
		FVector NewLocaction=GetSplineComponent()->GetLocationAtTime(_value,ESplineCoordinateSpace::Local,false);
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Ledg Block: %s"), *NewLocaction.ToString()));
		FRotator TargetRotaion=FRotator(GetActorRotation().Pitch+RotateBallSpeed,GetActorRotation().Yaw,GetActorRotation().Roll);
		SetActorLocation(NewLocaction);
		if(CharacterOwner)
		{
			if(CharacterOwner->MoveForwardFx > 0.2)
			{
				SetActorRotation(TargetRotaion);
			}

		}


		DistanceBall=UKismetMathLibrary::Vector_Distance(GetActorLocation(),TargetLocation);
		//if(DistanceBall)
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


void AFxBallActor::OnOverlapActivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	////todo check he has a defence skil
	AFxBaseCharacter * CharLoc=Cast<AFxBaseCharacter>(OtherActor);
	if(CharLoc)
	{
		CharLoc->HaveBallFx(this);

	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We got a collision."));
}


// Called when the game starts or when spawned
void AFxBallActor::BeginPlay()
{
	Super::BeginPlay();
	InitBall();

}

// Called every frame
void AFxBallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsBallCurve)
	{
		CurveFTimeline.TickTimeline(DeltaTime);
	}
	else
	{
		UpdateBallLocation();
	}

}

