// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Abilitys/GravityActor.h"
#include "GameFramework/Actor.h"
#include "Actors/Scatter/ScatterBall.h"
#include "ScatterMine.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AScatterMine : public AGravityActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AScatterMine();

	void Init(AActor* TargetActor, AActor* ShootingActor, FVector LocationToSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Target", Keywords = "TopDownRTSTemplate Target"), Category = TopDownRTSTemplate)
	AActor* Target;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	AActor* Shooter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TriggerCapsule", Keywords = "TopDownRTSTemplate TriggerCapsule"), Category = TopDownRTSTemplate)
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Damage = 40.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float LifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float MaxLifeTime = 30.f;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float MovementSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsFriendly = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsExplodable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsExplodableWhileGravity = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool DespawnAfterMaxLifeTime = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool SpawnComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float ScaleScatterBall = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int ScatterBallCount= 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector SpawnOffset = FVector(0.f,0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScatterBallMaterial", Keywords = "ScatterBallMaterial TopDownRTSTemplate"), Category = TopDownRTSTemplate)
	UMaterialInterface* ScatterBallMaterial;
	
	TArray<AScatterBall*> ScatterBalls;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnScatterBalls();

};
