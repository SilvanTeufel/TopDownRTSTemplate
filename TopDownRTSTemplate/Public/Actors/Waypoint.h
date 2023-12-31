// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Waypoint.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AWaypoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaypoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	AWaypoint* NextWaypoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
