// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "CompassPole.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ACompassPole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACompassPole();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	UStaticMeshComponent* PoleMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	FVector Direction = FVector(5.f, 0.f,0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	float MovementSpeed = 15.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	bool PoleIsMoving = false;

	UPROPERTY(BlueprintReadWrite, Category = "Compass")
	bool InvertMoving = false;

	float Timer = 0.f;
};
