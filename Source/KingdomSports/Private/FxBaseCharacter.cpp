// Fill out your copyright notice in the Description page of Project Settings.


#include "FxBaseCharacter.h"

// Sets default values
AFxBaseCharacter::AFxBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called when the game starts or when spawned
void AFxBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

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

