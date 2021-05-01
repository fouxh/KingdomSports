
#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include <GameFramework/DamageType.h>

#include "GameplayTagContainer.h"
#include "Curves/CurveVector.h"
#include "Templates/SubclassOf.h"
#include "FXData.generated.h"

/**
 *
 */
//ALS_MovementState
class UAnimSequenceBase;
class UAnimMontage;
class UCurveFloat;

//Movement states


UENUM(BlueprintType)
enum class EModificationTypeFx : uint8
{
	EAdd  UMETA(DisplayName = "Add"),
    EPercent   UMETA(DisplayName = "Percent"),
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EPoint  UMETA(DisplayName = "Point Damage"),
    EArea   UMETA(DisplayName = "Area Damage"),
};


UENUM(BlueprintType)
enum class EAiStateFx : uint8
{
	None   = 0     UMETA(DisplayName = "None"),
    Patrol 		UMETA(DisplayName = "Patrol"),
    Finding			UMETA(DisplayName = "Finding"),
    Investigations			UMETA(DisplayName = "Investigations"),
    Attacking			UMETA(DisplayName = "Attacking"),
    Evading			UMETA(DisplayName = "Evading"),
	Escaping			UMETA(DisplayName = "Escaping"),
	Stunned			UMETA(DisplayName = "Stunned"),
	Die			UMETA(DisplayName = "Die")

};
UENUM(BlueprintType)
enum class EBowStateFx : uint8
{
	Idle   = 0     UMETA(DisplayName = "Idle"),
    StartAim 		UMETA(DisplayName = "StartAim"),
    Aiming			UMETA(DisplayName = "Aiming"),
    Shooting			UMETA(DisplayName = "Shooting"),
    Recover			UMETA(DisplayName = "Recover")

};
//Movement states
UENUM(BlueprintType)
enum class EMovementStateFx : uint8
{
	Walking   = 0     UMETA(DisplayName = "Walking"),
    Running 		UMETA(DisplayName = "Running"),
    Sprinting			UMETA(DisplayName = "Sprinting"),
    Crouch			UMETA(DisplayName = "Crouch"),
    StartClimbing			UMETA(DisplayName = "StartClimbing"),
    Climbing			UMETA(DisplayName = "Climbing"),
	EndClimbing			UMETA(DisplayName = "EndClimbing")
};
//state of combat or not
UENUM(BlueprintType)
enum class ECombatStateFx : uint8
{
	NoneArmed = 0       UMETA(DisplayName = "NoneArmed"),
	ArmedBow        UMETA(DisplayName = "ArmedBow"),
	//add more status in case more weapons addedd



};

UENUM(BlueprintType)
enum class EItemTypeFx : uint8
{
	Weapon   = 0     UMETA(DisplayName = "Weapon"),
    Potion 		UMETA(DisplayName = "Potion"),
    Skill		UMETA(DisplayName = "Skill"),
   };
UENUM(BlueprintType)
enum class EWeaponTypeFx : uint8
{
	EMelee   = 0     UMETA(DisplayName = "Melee"),
    ERanged 		UMETA(DisplayName = "Ranged"),
    EMagic		UMETA(DisplayName = "Magic"),

   };
UENUM(BlueprintType)
enum class ETypeCurveFx : uint8
{
	EXPLevel   = 0     UMETA(DisplayName = "XPLevel"),
    ELevelDamageCurve 		UMETA(DisplayName = "LevelDamageCurve"),
    ELevelHealthCurve		UMETA(DisplayName = "LevelHealthCurve"),
    EGrantedXpCurve		UMETA(DisplayName = "GrantedXpCurve")
 };
UENUM(BlueprintType)
enum class EIntentDirectionTypeFx : uint8
{
	None   = 0     UMETA(DisplayName = "None"),
    Left 		UMETA(DisplayName = "Left"),
    Right		UMETA(DisplayName = "Right"),
    UPLeft		UMETA(DisplayName = "UPLeft"),
    UPRight		UMETA(DisplayName = "UPRight"),
    DownLeft		UMETA(DisplayName = "DownLeft"),
    DownRight		UMETA(DisplayName = "DownRight"),
    Up		UMETA(DisplayName = "Up"),
    Down		UMETA(DisplayName = "Down")
 };
UENUM(BlueprintType)
enum class EdgeTypeFX : uint8
{
	Left 		UMETA(DisplayName = "Left"),
    Right		UMETA(DisplayName = "Right"),

 };
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	ENone = 0     UMETA(DisplayName = "None"),
	EPassive      UMETA(DisplayName = "Passive"),
    EMagic      UMETA(DisplayName = "Magic"),
    EHeavy    UMETA(DisplayName = "Heavy Attack"),
    EAeo     UMETA(DisplayName = "Area Damage"),
    EUltimate     UMETA(DisplayName = "Ultimate"),
    EFatality     UMETA(DisplayName = "Fatality"),

};



USTRUCT(BlueprintType)
struct FMovementSettingsFx
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    float WalkSpeed=180.0 ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    float RunSpeed =360.0;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    float SprintSpeed=600.0 ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    float CrouchSpeed=180.0 ;

};
USTRUCT(BlueprintType)
struct FCameraSettingsFx : public FTableRowBase
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
	float TargetArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	FVector SocketOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	bool bislagEnabled=true;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
	float LagSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	float RotationLagSpeed;
	//Camera Fvo
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	float FOV =90.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	bool DoCollisionTest=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	bool bisCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	TSubclassOf<class UCameraShake> CameraShake;



};

USTRUCT(BlueprintType)
struct FActionFx : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	FGameplayTag ActionTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	UAnimMontage * AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	bool HasCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	float CoolDown;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = Fx)
	class UFXSpawnedAction* Action;*/

};

USTRUCT(BlueprintType)
struct FAttachedINfo : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	FName SheathSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	FName InHandSocket;

};

USTRUCT(BlueprintType)
struct FWeaponEffects : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ACF)
	class USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ACF)
	class UParticleSystem* AttackParticle;

};
USTRUCT(BlueprintType)
struct FHitActors
{
	GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = FX)
    TArray<class AActor*> AlreadyHitActors;

};
USTRUCT(BlueprintType)
struct FTraceInfo
{
	GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FX)
    FName StartSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FX)
    FName EndSocket;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FX)
    float Radius;

	/** Select if it's Area or Point Damage*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = bAutoApplyDamage), Category = ACM)
    EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    class USoundCue* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    class UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    float TrailLength = 1.f;

	/** The type of damage applied*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bAutoApplyDamage), Category = ACM)
    TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

	/** The base damage to apply to the actor (Can be modified in you TakeDamage Implementation)*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bAutoApplyDamage), Category = ACM)
    float BaseDamage = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = bAutoApplyDamage), Category = ACM)
    float FinalDamage = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = FX)
	TEnumAsByte<ECollisionChannel> TraceChannelFx;
};

USTRUCT(BlueprintType)
struct FGameCurvesFx
{
	GENERATED_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
UCurveFloat * XPLevelCurve;
UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
UCurveFloat * LevelDamageCurve;
UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
UCurveFloat * LevelHealthCurve;
UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
UCurveFloat * GrantedXpCurve;
};
USTRUCT(BlueprintType)
struct FGameCurvesTagsFx
{
	GENERATED_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
	FGameplayTag TagCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Fx)
    UCurveFloat * CurveFloatFx;

};
USTRUCT(BlueprintType)
struct FSkillsInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
     ESkillType SKillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
     FName SKillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
     FText SKillDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
     UObject * SkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    int RequiredLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    FGameplayTag ActionTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    FGameplayTag WeaponTag;

};

//////Quest System//////////////
///
UENUM(BlueprintType)
enum class EQuestStatusFx : uint8
{
	EAll UMETA(DisplayName = "All"),
	ECompleted UMETA(DisplayName = "Completed"),
	EFailed UMETA(DisplayName = "Failed"),
	ECurrent UMETA(DisplayName = "Current"),


   };
UENUM(BlueprintType)
enum class EObjectTypeFx : uint8
{
	EKill UMETA(DisplayName = "Kill"),
    EReachDestination UMETA(DisplayName = "ReachDestination"),
    EPickUp UMETA(DisplayName = "PickUp"),
    EDelivery UMETA(DisplayName = "Delivery"),
    ETimed UMETA(DisplayName = "Timed"),
    ECustom UMETA(DisplayName = "Custom"),

   };
UENUM(BlueprintType)
enum class ERewordTypeFx : uint8
{
	EGold UMETA(DisplayName = "Gold"),
    EGem UMETA(DisplayName = "Gem"),
    EXP UMETA(DisplayName = "XP"),
    ESkillPoint UMETA(DisplayName = "SkillPoint"),
    EIron UMETA(DisplayName = "Iron"),
    EWood UMETA(DisplayName = "Wood"),
    EPotion UMETA(DisplayName = "Potion"),
    EFood UMETA(DisplayName = "EFood"),

   };
USTRUCT(BlueprintType)
struct FRewordInfo: public FTableRowBase
{
	GENERATED_BODY()
 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fx)
 ERewordTypeFx RewordType ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
     FText Name ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
        int Number ;

};
USTRUCT(BlueprintType)
struct FRewordList: public FTableRowBase
{
	GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
     TArray<FRewordInfo> RewordList;

};
USTRUCT(BlueprintType)
struct FQuestInfoFx: public FTableRowBase
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fx)
     FText QuestName ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
     FText QuestDescription ;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
     int CurrentIndex ;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = Fx)
   TArray<class UFXBaseObjectiveAction*>  ObjectivesList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = Fx)
   class UFXEndQuestAction*  EndQuestAction;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       int RecommendedLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       FRewordList RewordList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       bool IsActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       FTransform PlayerSpawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       FName MapQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       int QuestSequenceNumber;


};

USTRUCT(BlueprintType)
struct FSpawnInfo: public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
     TSubclassOf<class AActor>  ActorSpawned;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
     int TotalNumber=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    float RadiusSpawn=300.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    FTransform SpawnLocation;

};
USTRUCT(BlueprintType)
struct FSpawnlist: public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       TArray<FSpawnInfo> SpawnData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    float TimeToWaitAfterWave=3.0;


};

//////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FPlayerInfoFx : public FTableRowBase
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    int Level;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    int XP;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    int SkillPoints;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    int Gold;;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    int Gems;;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    FName MapName;;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    FTransform CheckPointLocation;;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Fx)
    TArray<FName> Skills;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
    int CurrentObjectiveIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       FQuestInfoFx CurrentQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       TArray<FQuestInfoFx> CurrentQuestList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       TArray<FQuestInfoFx> CompletedQuestList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FX)
       TArray<FQuestInfoFx> FailedQuestList;

};
UCLASS()
class KINGDOMSPORTS_API UFXData : public UObject
{
	GENERATED_BODY()

};
