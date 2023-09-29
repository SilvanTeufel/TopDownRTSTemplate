// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ExtendedCameraBase.h"
#include "Actors/Compass/CompassShell.h"
#include "Actors/Compass/CompassPin.h"
#include "CompassCamera.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ACompassCamera : public AExtendedCameraBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "Compass")
	TSubclassOf<class ACompassShell> CompassShellClass;

public:
	UPROPERTY(EditAnywhere, Category = "Compass")
	class ACompassShell* CompassShell;
	
	UPROPERTY(EditAnywhere, Category = "Compass")
	FVector ShellScaler = FVector(0.5f, 0.5f, 0.5f);
	
	UFUNCTION(BlueprintCallable, Category = "Compass")
	void CreateCompassShell();

	UFUNCTION(BlueprintCallable, Category = "Compass")
	void PositionCompassShell();
	
	UPROPERTY(EditAnywhere, Category = "Compass")
	TArray<class ACompassPin*> CompassPins;

	UPROPERTY(EditAnywhere, Category = "Compass")
	FVector PinScaler = FVector(0.5f,0.5f,0.5f);
	
	UFUNCTION(BlueprintCallable, Category = "Compass")
	void SpawnCompassPins();

	UFUNCTION(BlueprintCallable, Category = "Compass")
	void PositionCompassPins();

	UPROPERTY(EditAnywhere, Category = "Compass")
	FVector PositionOffset = FVector( -150.f, -50.f, -150.f);
};
