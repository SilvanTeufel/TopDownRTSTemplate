// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "HoldPositionDetector.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AHoldPositionDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHoldPositionDetector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void InitDetector(AActor* ShootUnit);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void ScaleDetectors(FVector Scaler, float Range);
	
	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	FVector DetectorScaler = FVector(5.f, 5.f, 1.f);

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	AActor* ShootingUnit;
};
