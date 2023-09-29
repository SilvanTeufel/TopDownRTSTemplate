// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Unit/UnitControllerBase.h"
#include "FogExplorerController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AFogExplorerController : public AUnitControllerBase
{
	GENERATED_BODY()
public:
	AFogExplorerController();
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectedFog(const TArray<AActor*>& DetectedFog);
};
