// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CompassPole.h"
#include "Components/StaticMeshComponent.h"
#include "CompassPin.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ACompassPin : public AActor
{
	GENERATED_BODY()
	
public:
	ACompassPin();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	UStaticMeshComponent* PinMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	ACompassPole* CompassPole;

	UPROPERTY(EditAnywhere, Category = "Compass")
	TSubclassOf<class ACompassPole> CompassPoleClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	FVector CompassPoleLocation = FVector(10000,0.f,50.f);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	FVector PinMeshSize = FVector(0.5f, 0.5f, 0.5f);
	
	UFUNCTION(BlueprintCallable, Category = "Compass")
	void SetCompassPole(ACompassPole* NewCompassPole);

	UPROPERTY(EditAnywhere, Category = "Compass")
	FRotator PinRotation = FRotator::ZeroRotator;
};
