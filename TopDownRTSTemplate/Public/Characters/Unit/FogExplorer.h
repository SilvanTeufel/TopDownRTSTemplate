// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PathSeekerBase.h"
#include "Components/CapsuleComponent.h"
#include "Actors/FogDetector.h"
#include "FogExplorer.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AFogExplorer : public APathSeekerBase
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	bool IsVisibleInFog = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	bool IsVisible = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	//int OverlapCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	class UCapsuleComponent* TriggerCapsule;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void DetectActorsInRadius(float Radius);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnFogDetector();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	TSubclassOf<AFogDetector> FogDetectorClass;
};
