// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FxBallActor.h"
#include "FXData.h"
#include "GameFramework/Character.h"
#include "FxBaseCharacter.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallStatusChange, EPlayerBallStatusFx, BallStatus_loc);
UCLASS()
class KINGDOMSPORTS_API AFxBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFxBaseCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = FX)
	class UArrowComponent * ArrowDirection;
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadwrite, Category = FX)
	AFxBallActor* BallRefFx;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = FX)
    EIntentDirectionTypeFx DirectionIntent;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
    float MoveForwardFx;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
    float MoveRightFx;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
    float MaxBallDistance=120.0;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
    float MinBallDistance=30.0;
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadwrite, Category = State)
    EPlayerBallStatusFx PlayerBallStatus;
	UFUNCTION(BlueprintPure,Category=FX)
    EIntentDirectionTypeFx GetDirection();
	UFUNCTION(BlueprintCallable,Category=State)
    void InitCharacter();
	UFUNCTION(BlueprintCallable,Category=State)
	void HaveBallFx(AFxBallActor* Ball_loc);
	UFUNCTION(Server,Reliable,BlueprintCallable,Category=State)
	void UpdateBallStatus(EPlayerBallStatusFx _sts);
	UFUNCTION(BlueprintCallable,Category=Fx)
    void StartMoveWithTheBall();
	UFUNCTION(Server,Reliable,BlueprintCallable,Category=State)
	void MovingWithBall_srv(FVector _Loc);
	UFUNCTION(Server,Reliable,BlueprintCallable,Category=State)
	void SetBallRef(AFxBallActor* Ball_loc);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintAssignable, Category = FX)
	FOnBallStatusChange  OnBallStatusChange ;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
