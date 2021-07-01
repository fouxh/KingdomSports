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
/*	DOREPLIFETIME(AFxBallActor, IsBallMove);
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
		if (CharacterOwner)
		{
			if (CharacterOwner->MoveForwardFx > 0.2)
				SetActorRotation(TargetRotaion);
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
	/*
	AMyGKCharacter* GK = Cast<AMyGKCharacter>(CharacterOwner);
	if (GK)
	{
		if (IsAttachedTo(CharacterOwner))
		{
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Yellow, TEXT("attached"));
			return;
		}
		//SetActorLocation(GK->GetMesh()->GetSocketLocation("BallHand"));
		AttachToComponent(GK->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true), "BallHand");
		return;
	}
	*/
	if(IsBallMove)
	{
		FVector MoveLocation_loc=UKismetMathLibrary::VLerp(StartLocation,TargetLocation,BallSpeed);
		DistanceBall=UKismetMathLibrary::Vector_Distance(GetActorLocation(),TargetLocation);
		if(DistanceBall<=2.0)
		{
			RotateBallSpeed=0.001;
		}
		else
		{
			RotateBallSpeed=5;
		}
		FRotator TargetRotation=FRotator(GetActorRotation().Pitch,GetActorRotation().Yaw,GetActorRotation().Roll+RotateBallSpeed);
		//SetActorLocationAndRotation(MoveLocation_loc, TargetRotation);
		SetActorLocation(MoveLocation_loc);
	}
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

void AFxBallActor::UpdatePhysics_srv_Implementation(bool _Enable)
{
	BallCollision->SetSimulatePhysics(_Enable);
	BallCollision->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	UpdatePhysics_Multi(_Enable);
}

void AFxBallActor::UpdatePhysics_Multi_Implementation(bool _Enable)
{
	BallCollision->SetSimulatePhysics(_Enable);
	BallCollision->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ALOO"));
}

void AFxBallActor::OnOverlapActivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AFxBaseCharacter* CharLoc = Cast<AFxBaseCharacter>(OtherActor);
	if (CharLoc) 
	{
		if (CharLoc == CharacterOwner || CharLoc->IsStunned)
		{
			return;
		}

		if (!CharLoc->IsStunned)
		{
			CharLoc->SetBallRef(this);
			CharLoc->UpdateBallStatus(EPlayerBallStatusFx::EPossessBall);
			CharLoc->UpdateTick(true);
			CharLoc->SetActorTickEnabled(true);

			for (int i = 0; i < PlayerArray_Out.Num(); ++i)
			{
				AFxBaseCharacter* Player_locref = Cast<AFxBaseCharacter>(PlayerArray_Out[i]);
				if (Player_locref)
				{
					if (Player_locref != CharLoc)
					{
						Player_locref->UpdateTick(false);
						Player_locref->SetActorTickEnabled(false);
						Player_locref->UpdateBallStatus(EPlayerBallStatusFx::ENone);
					}
				}
			}
		}

		/*if (BallCollision->IsSimulatingPhysics())
		{
			CharLoc->ResetBallPhysics();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Simulating"));
		}*/
	}
}

void AFxBallActor::BeginPlay()
{
	Super::BeginPlay();
	InitBall();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFxBaseCharacter::StaticClass(), PlayerArray_Out);
}

void AFxBallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		if (IsBallCurve)
		{
			CurveFTimeline.TickTimeline(DeltaTime);
		}
		else
		{
			UpdateBallLocation();
		}
}