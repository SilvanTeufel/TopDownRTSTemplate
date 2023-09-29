// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HoldPositionUnit.h"
#include "Characters/Unit/UnitBase.h"
#include "Actors/Laser/LaserBeam.h"
#include "Actors/Laser/LaserPrism.h"
#include "LaserUnit.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ALaserUnit : public AHoldPositionUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Laser Unit")
	void RestoreEnergy(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Laser Unit")
	void AddEnergy(float EnergyToAdd);
	
	UFUNCTION(BlueprintCallable, Category = "Laser Unit")
	void SetControlTimer(float Time);
	
	UFUNCTION(BlueprintCallable, Category = "Laser Unit")
	ALaserBeam* SpawnLaser(FVector Location, FRotator RotationOffset);
	
	UFUNCTION(BlueprintCallable, Category = "Laser Unit")
	void SpawnLaserIndicator(FVector Location, FRotator RotationOffset);

	UFUNCTION(BlueprintCallable, Category = "Laser Unit")
	void DestroyLaserIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
private:
	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	TSubclassOf<class ALaserBeam> LaserIndicatorClass;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	TArray<ALaserBeam*> PrismBeamIndicators;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float LaserFireTime = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float ControlTimer = 0.f;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	FVector LastForwardVector;
	
	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	TSubclassOf<class ALaserBeam> LaserBeamClass;
	

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	TArray<ALaserBeam*> LaserBeams;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	ALaserPrism* ActivePrism;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float RangeToPrism = 0.f;
	
public:
	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float LaserLoadTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	FVector SpawnOffset = FVector(0.f, 0.f, 50.f);

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	ALaserBeam* ActiveLaserIndicator;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float MaxEnergy = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float Energy = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float EnergyCost = 30.f;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float EnergyTimer = 0.f;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float EnergyRestoreTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	float EnergyToRestore = 1.f;

	UPROPERTY(EditAnywhere, Category = "Laser Unit")
	int LaserCount = 0;
};
