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
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadwrite, Category = FX)
		AFxBallActor* BallRefFx;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadwrite, Category = FX)
		AActor* Target;
	UPROPERTY(BlueprintReadwrite)
		TArray<AActor*> PlayerArray_Out;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadwrite)
		bool IsStunned;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = FX)
		EIntentDirectionTypeFx DirectionIntent;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
		float MoveForwardFx;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
		float MoveRightFx;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
		float MaxBallDistance = 120.0;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = State)
		float MinBallDistance = 30.0;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		FVector GoalTargetLocation;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadwrite, Category = State)
		EPlayerBallStatusFx PlayerBallStatus;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadwrite)
		bool Bicycle;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		bool IsRed;
//////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintPure, Category = FX)
		EIntentDirectionTypeFx GetDirection();
	UFUNCTION(BlueprintCallable, Category = State)
		void InitCharacter();
	//UFUNCTION(BlueprintCallable, Category = Fx)
	//	void StartMoveWithTheBall();
	UFUNCTION(BlueprintCallable)
		void GoalTarget(float DeltaTime);
	//UFUNCTION(BlueprintCallable)
	//	void MoveBall(FVector Location);
	//UFUNCTION(BlueprintCallable)
	//	void BallMovement(FVector Location);
	UFUNCTION(BlueprintImplementableEvent)
		void AttachBall(class AFxBallActor* ball);
//////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void ResetBallPhysics();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = State)
		void SetBallRef(AFxBallActor* Ball_loc);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = State)
		void UpdateTick(bool _Enable);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = State)
		void UpdateBallStatus(EPlayerBallStatusFx _sts);
	//UFUNCTION(Server, Reliable, BlueprintCallable)
	//	void MoveBall_Srv(FVector Location);
	//UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	//	void MoveBall_Multi(FVector Location);
	//UFUNCTION(Client, Reliable, BlueprintCallable)
	//	void MoveBall_Client(FVector Location);
	//UFUNCTION(Server, Reliable, BlueprintCallable)
	//	void PlayMontage_Srv(UAnimMontage* Animation);
	//UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	//	void PlayMontage_Multi(UAnimMontage* Animation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintAssignable, Category = FX)
	FOnBallStatusChange OnBallStatusChange;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};