// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "UnitData.generated.h"

UENUM()
namespace UnitData
{
	enum EState
	{
		Idle     UMETA(DisplayName = "Idle"),
		Run     UMETA(DisplayName = "Run"),
		Patrol UMETA(DisplayName = "Patrol"),
		Jump   UMETA(DisplayName = "Jump"),
		Attack UMETA(DisplayName = "Attack"),
		Healing UMETA(DisplayName = "Healing"),
		Pause UMETA(DisplayName = "Pause"),
		Chase UMETA(DisplayName = "Chase"),
		Charge UMETA(DisplayName = "Charge"),
		IsAttacked UMETA(DisplayName = "IsAttacked"),
		Stunned UMETA(DisplayName = "Stunned"),
		HoldPositionPause UMETA(DisplayName = "HoldPositionPause"),
		HoldPositionAttack UMETA(DisplayName = "HoldPositionAttack"),
		HoldPositionReload UMETA(DisplayName = "HoldPositionReload"),
		HoldPositionWatch UMETA(DisplayName = "HoldPositionWatch"),
		HoldPositionCreate UMETA(DisplayName = "HoldPositionCreate"),
		Dead UMETA(DisplayName = "Dead"),
		ThrowWeapon UMETA(DisplayName = "ThrowWeapon"),
		WeaponGravity UMETA(DisplayName = "WeaponGravity"),
		UseGravity UMETA(DisplayName = "UseGravity"),
		Reload UMETA(DisplayName = "Reload"),
		TeleportStart UMETA(DisplayName = "TeleportStart"),
		TeleportEnd UMETA(DisplayName = "TeleportEnd"),
		PrepareScatterMine UMETA(DisplayName = "PrepareScatterMine"),
		PrepareMouseBot UMETA(DisplayName = "PrepareMouseBot"),
		NoMana UMETA(DisplayName = "NoMana"),
		LaserLoading UMETA(DisplayName = "LaserLoading"),
		LaserFire UMETA(DisplayName = "LaserFire"),
		None UMETA(DisplayName = "None"),
	};

}

USTRUCT(BlueprintType)
struct FSpeechData_Texts : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate, meta = (MultiLine=true))
	FText Text;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	USoundBase* SpeechSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float BlendPoint_1 = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float BlendPoint_2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Time = 5.f;
	
};

USTRUCT(BlueprintType)
struct FSpeechData_Buttons : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int Text_Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate, meta = (MultiLine=true))
	FText Text;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int New_Text_Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	USoundBase* ButtonSound;
	
};


UENUM()
namespace BotData
{
	enum BotStatus
	{
		Idle_Bot     UMETA(DisplayName = "Idle_Bot"),
		Run_Bot     UMETA(DisplayName = "Run_Bot"),
		Climb_Bot   UMETA(DisplayName = "Climb_Bot"),
		Attack_Bot   UMETA(DisplayName = "Attack_Bot"),
		Dead_Bot  UMETA(DisplayName = "Dead_Bot"),
		Heal_Bot UMETA(DisplayName = "Heal_Bot"),
	};
}

UENUM()
namespace WeaponStates
{
	enum WeaponState
	{
		Flying     UMETA(DisplayName = "Flying"),
		Returning     UMETA(DisplayName = "Returning"),
		Normal   UMETA(DisplayName = "Normal"),
	};
}

UENUM()
namespace CameraData
{
	enum CameraState
	{
		UseScreenEdges     UMETA(DisplayName = "UseScreenEdges"),
		MoveWASD  UMETA(DisplayName = "MoveWASD"),
		RotateToStart  UMETA(DisplayName = "RotateToStart"),
		MoveToPosition  UMETA(DisplayName = "MoveToPosition"),
		OrbitAtPosition  UMETA(DisplayName = "OrbitAtPosition"),
		ZoomIn  UMETA(DisplayName = "ZoomIn"),
		ZoomOut  UMETA(DisplayName = "ZoomOut"),
		ScrollZoomIn  UMETA(DisplayName = "ScrollZoomIn"),
		ScrollZoomOut  UMETA(DisplayName = "ScrollZoomOut"),
		ZoomOutPosition  UMETA(DisplayName = "ZoomOutPosition"),
		ZoomInPosition  UMETA(DisplayName = "ZoomInPosition"),
		HoldRotateLeft UMETA(DisplayName = "HoldRotateLeft"),
		HoldRotateRight UMETA(DisplayName = "HoldRotateRight"),
		RotateLeft UMETA(DisplayName = "RotateLeft"),
		RotateRight UMETA(DisplayName = "RotateRight"),
		LockOnCharacter UMETA(DisplayName = "LockOnCharacter"),
		LockOnSpeaking UMETA(DisplayName = "LockOnSpeaking"),
		ZoomToNormalPosition  UMETA(DisplayName = "ZoomToNormalPosition"),
		ThirdPerson UMETA(DisplayName = "ThirdPerson"),
		ZoomToThirdPerson UMETA(DisplayName = "RotateBeforeThirdPerson"),
	};
}


UENUM()
namespace SelectableData
{
	enum SelectableType
	{
		Health     UMETA(DisplayName = "Health"),
		Mana   UMETA(DisplayName = "Mana "),
		Ammo     UMETA(DisplayName = "Ammo"),
		Magazine UMETA(DisplayName = "Magazine"),
		WeaponTalentPoint UMETA(DisplayName = "WeaponTalentPoint"),
	};
}

UENUM()
namespace DetectorData
{
	enum DetectorType
	{
		Speech     UMETA(DisplayName = "Speech"),
		UnitSpawn   UMETA(DisplayName = "UnitSpawn"),
		UnitChaseSpawn   UMETA(DisplayName = "UnitChaseSpawn"),
		SelectableSpawn    UMETA(DisplayName = "SelectableSpawn"),
	};
}

inline float CheckAngle(FVector Vector, float Angle) {

	float X = Vector.X;
	float Y = Vector.Y;

	if (Vector.X < 0.001 && Vector.X > -0.001 && Vector.Y > 0.000) {
		Angle = 90;
	}
	else if (Vector.X < 0.001 && Vector.X > -0.001 && Vector.Y < 0.000) {
		Angle = 270;
	}
	else if (Vector.Y < 0.001 && Vector.Y > -0.001 && Vector.X > 0.000) {
		Angle = 0.;
	}
	else if (Vector.Y < 0.001f && Vector.Y > -0.001 && Vector.X < 0.000) {
		Angle = 180;
	}
	else if (Vector.X > 0.000 && Vector.Y > 0.000) {
		Angle += 0.000;
	}
	else if (Vector.X < 0.000 && Vector.Y > 0.000) {
		Angle += 0.000;
	}
	else if (Vector.X > 0.000 && Vector.Y < 0.000) {
		Angle = 360 - Angle;
	}
	else if (Vector.X < 0.000 && Vector.Y < 0.000) {
		Angle = 360 - Angle;
	}
	else {
	}

	return Angle;
};

inline float CalculateAngle(const FVector& VectorOne, const FVector& VectorTwo)
{
	FVector NormalizedVectorOne = VectorOne.GetSafeNormal();
	FVector NormalizedVectorTwo = VectorTwo.GetSafeNormal();

	float AngleInRadians = FMath::Acos(FVector::DotProduct(NormalizedVectorOne, NormalizedVectorTwo));
	float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

	// Calculate the sign of the angle using the cross product
	FVector CrossProduct = FVector::CrossProduct(NormalizedVectorOne, NormalizedVectorTwo);
	float Sign = FMath::Sign(CrossProduct.Z);

	// Apply the sign to the angle
	AngleInDegrees *= Sign;

	return AngleInDegrees;
}


inline FVector RotateVectorAroundPoint(const FVector& VectorToRotate, const FVector& PointToRotateAround, const FRotator& Rotation)
{
	FVector rotatedVector = VectorToRotate - PointToRotateAround;  // Verschiebe den Vektor zum Rotieren um den Ursprung des Koordinatensystems
	rotatedVector = Rotation.RotateVector(rotatedVector);  // Drehe den Vektor um den Rotationswinkel
	rotatedVector += PointToRotateAround;  // Verschiebe den Vektor zurück zum ursprünglichen Punkt
	return rotatedVector;
}