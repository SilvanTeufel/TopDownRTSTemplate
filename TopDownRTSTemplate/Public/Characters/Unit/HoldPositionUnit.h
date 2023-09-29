// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/SpeakingUnit.h"
#include "Actors/Abilitys/HoldPositionDetector.h"
#include "HoldPositionUnit.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AHoldPositionUnit : public ASpeakingUnit
{
	GENERATED_BODY()

public:
	
	
	UPROPERTY(EditAnywhere, Category = TopdownRTSTemplate)
	TSubclassOf<class AHoldPositionDetector> DetectorClass;

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void SpawnDetector();

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void DetectorUnLock();
	
	UPROPERTY(EditAnywhere, Category = TopdownRTSTemplate)
	AHoldPositionDetector* Detector;

	UPROPERTY(EditAnywhere, Category = TopdownRTSTemplate)
	FRotator DetectorRotation = FRotator(0.f,0.f,0.f);

	UPROPERTY(EditAnywhere, Category = TopdownRTSTemplate)
	FVector DetectorLocationOffset = FVector(0.f,0.f, 30.f);

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "UnitsToChase", Keywords = "TopDownRTSTemplate UnitsToChase"), Category = TopDownRTSTemplate)
	TArray<AUnitBase*> UnitsDetected;

	UPROPERTY(BlueprintReadWrite, Category = RTSUnitTemplate)
	AUnitBase* UnitToShoot;
};
