// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "FxBallActor.generated.h"

UCLASS()
class KINGDOMSPORTS_API AFxBallActor : public AActor
{
	GENERATED_BODY()
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
 //       class USplineComponent* SplineComponentFx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
        class UStaticMeshComponent * BallMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
        class USphereComponent* BallCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* Root;
public:
	// Sets default values for this actor's properties
	AFxBallActor();

	//UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
	//bool IsBallCurve;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FVector StartLocation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FVector TargetLocation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    float RotateBallSpeed=5;
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
 //   float DistanceBall;
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
 //   float MaxDistanceBall=120.0;
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite)
    class AFxBaseCharacter* CharacterOwner;
	UPROPERTY(BlueprintReadwrite)
	class AGameStateBase* Gamestate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RedOwned;
/////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable,Category=Fx)
	void InitBall();
	//UFUNCTION(BlueprintCallable,Category=Fx)
 //   void FinishMoveBallCurve();
	//UFUNCTION(BlueprintCallable,Category=Fx)
 //   void TimelineProgress(float _value);
	//UFUNCTION(BlueprintCallable,Category=Fx)
	//void Moveball_Curve(FVector Target_loc,float offsetZ,float offsetX);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void DisablePhysics_srv();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void DisablePhysics_Multi();
	UFUNCTION(BlueprintCallable,Category=Fx)
	void OnOverlapActivateSphere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//////////TimeLine
	//FTimeline CurveFTimeline;
	//FOnTimelineEvent OnFinishedTimeline;
	//UPROPERTY(EditAnywhere, Category = "Timeline")
 //   UCurveFloat* CurveFloat;
	//UPROPERTY()
 //   FVector StartLoc;
	//UPROPERTY()
 //   FVector EndLoc;
	//UPROPERTY(EditAnywhere, Category = "Timeline")
 //   float ZOffset;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//UFUNCTION(BlueprintPure, Category = FX)
 //       FORCEINLINE class USplineComponent* GetSplineComponent() const { return SplineComponentFx; }
	UFUNCTION(BlueprintPure, Category = FX)
        FORCEINLINE class UStaticMeshComponent* GetBallMesh() const { return BallMesh; }
};
