// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Talents.generated.h"

USTRUCT(BlueprintType)
struct FWeaponTalents : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int WeaponId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float RangeOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int MagOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float AttackSpeedScaler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int PiercedTargetsOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float ProjectileScaler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float ProjectileSpeedScaler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float WeaponStunOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int LaserCountOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int ShieldCountOffset;
};

USTRUCT()
struct FWeaponTalentPoints
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int WeaponId;
	
	UPROPERTY()
	int RangeOffset;

	UPROPERTY()
	int MagOffset;
	
	UPROPERTY()
	int AttackSpeedScaler;
	
	UPROPERTY()
	int PiercedTargetsOffset;

	UPROPERTY()
	int ProjectileScaler;

	UPROPERTY()
	int ProjectileSpeedScaler;

	UPROPERTY()
	int WeaponStunOffset;

	UPROPERTY()
	int LaserCountOffset;

	UPROPERTY()
	int ShieldCountOffset;
};

USTRUCT()
struct FWeaponTalentData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FWeaponTalents Talents;
	
	UPROPERTY()
	FWeaponTalentPoints TalentPoints;
};


USTRUCT()
struct FLaserTalents
{
	GENERATED_USTRUCT_BODY()
};