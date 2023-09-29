// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Laser/LaserBeam.h"
#include "LaserPrism.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ALaserPrism : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserPrism();

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	UStaticMeshComponent* PrismMesh;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	FRotator Rotation = FRotator(0.f,0.f, 0.f);

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	ALaserBeam* OutBeam = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	ALaserBeam* InBeam = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TSubclassOf<class ALaserBeam> LaserBeamClass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnPrismOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnWithTimer(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float SpawnControlTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float SpawnCoolDown = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool SpawnNewLaser = false;
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnPrismOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	ALaserBeam* SpawnLaser(FVector Location, FRotator Rot);
};
