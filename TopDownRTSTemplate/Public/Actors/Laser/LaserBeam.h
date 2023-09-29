// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Unit/UnitBase.h"
#include "Materials/Material.h" // If you need to use materials
#include "Materials/MaterialInstanceDynamic.h" // For using UMaterialInstanceDynamic
#include "Core/UnitData.h"
#include "LaserBeam.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ALaserBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserBeam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	float LaserRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	float LaserMaterialChangeTime = 0.5f;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void ChangeLaserMaterial();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TArray<AUnitBase*> UnitBases;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void UpdateUnits(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void MoveLaser(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float ControlTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	float UpdateInterval = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	float LifeTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	float MaxLifeTime = 0.5f;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void KillAfterLifeTime(float DeltaTime);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	UStaticMeshComponent* LaserMesh;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	FVector Scaler = FVector(0.1f, 0.1f, 7.f);

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	FVector DirectionVector;
	
	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	float LaserDamage = 500.0f;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	float LaserIntervalDamage = 50.0f;
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void AddUnit(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void RemoveUnit(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnLaserOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnLaserEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//bool IsOverlapped = false;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	bool IsIndicator = false;
	
private:
	float TimeSinceMaterialChange;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	UMaterialInstanceDynamic* LaserMaterial;
};
