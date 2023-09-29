// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Unit/FogExplorerController.h"
#include "Core/UnitData.h"
#include "HoldPositionController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AHoldPositionController : public AFogExplorerController
{

	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
		void HoldPositionStateMachine(float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
		void HoldPositionWatch(AHoldPositionUnit* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable,Category = RTSUnitTemplate)
		void HoldPositionPause(AHoldPositionUnit* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
		void HoldPositionAttack(AHoldPositionUnit* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable,Category = RTSUnitTemplate)
		void HoldPositionReload(AHoldPositionUnit* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
		bool SetNextUnitToShoot(AHoldPositionUnit* UnitBase);

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "DistanceToUnitToChase", Keywords = "TopDownRTSTemplate DistanceToUnitToChase"), Category = TopDownRTSTemplate)
	float DistanceToShootingUnit = 0.0f;
};
