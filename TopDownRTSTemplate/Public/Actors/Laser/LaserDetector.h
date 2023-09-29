// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Actors/Laser/LaserBeam.h"

#include "LaserDetector.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ALaserDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserDetector();

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	UStaticMeshComponent* DetectorMesh;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	UMaterialInterface* BlueMaterial;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	UMaterialInterface* RedMaterial;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	UMaterialInstanceDynamic* DynMaterial;
	
	UFUNCTION(Category = TopDownRTSTemplate)
	void ChangeMaterialColour(FVector4d Colour);
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float ControlTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float DisableDetectorTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool Enabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool StartTick = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
