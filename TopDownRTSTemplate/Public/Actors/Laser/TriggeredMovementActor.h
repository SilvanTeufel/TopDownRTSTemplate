// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Laser/LaserDetector.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SceneComponent.h"

#include "TriggeredMovementActor.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ATriggeredMovementActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggeredMovementActor();


	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	UStaticMeshComponent* MoverMesh;
	
	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TArray<ALaserDetector*> LaserDetectorsAND;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TArray<ALaserDetector*> LaserDetectorsOR;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector Speed = FVector(0.f,0.f,4.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float DistanceTolerance = 0.f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void DetectorsAND();
	
	void DetectorsOR();
};
