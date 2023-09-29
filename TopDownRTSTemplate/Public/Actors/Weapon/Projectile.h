// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	void Init(AActor* TargetActor, AActor* ShootingActor);

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Target", Keywords = "TopDownRTSTemplate Target"), Category = TopDownRTSTemplate)
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TargetOffset", Keywords = "TopDownRTSTemplate TargetOffset"), Category = TopDownRTSTemplate)
	FVector TargetOffset = FVector(0.f,0.f,0.f); // Z = -11.5f
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Shooter", Keywords = "TopDownRTSTemplate Shooter"), Category = TopDownRTSTemplate)
	AActor* Shooter;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TriggerCapsule", Keywords = "TopDownRTSTemplate TriggerCapsule"), Category = TopDownRTSTemplate)
	//class UCapsuleComponent* TriggerCapsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector TargetLocation;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector ShooterLocation;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float RangeDampScaler = 0.2f;
	
	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Damage;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float LifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float MaxLifeTime = 2.f;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsFriendly = true;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float MovementSpeed = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float EnergyToAdd = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int MaxPiercedTargets = 1;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	int PiercedTargets = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	float CreateDamageWithRange(AExtendedUnitBase* ExtendedShooter, AUnitBase* UnitToHit);

};
