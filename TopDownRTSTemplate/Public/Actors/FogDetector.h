// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "FogDetector.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AFogDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogDetector();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UStaticMeshComponent* DetectorMesh;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void CheckForLos();
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void InitOverlaps();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	AActor* DetectorOwner;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetDetectorOwner(AActor* NewOwnerDetector) { DetectorOwner = NewOwnerDetector; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TArray<AActor*> DetectUnits;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//FTimerHandle TimerHandle_CheckForLos;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
