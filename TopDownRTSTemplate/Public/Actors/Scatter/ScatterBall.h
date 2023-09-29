// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScatterBall.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AScatterBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScatterBall();
	
	void Init(float BallDamage, float BallScale, bool XSpeedIsPos, bool YSpeedIsPos, UMaterialInterface* Material, AActor* Unit);
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TriggerCapsule", Keywords = "TopDownRTSTemplate TriggerCapsule"), Category = TopDownRTSTemplate)
	//class UCapsuleComponent* TriggerCapsule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	AActor* Shooter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Damage = 40.f;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float LifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float MaxLifeTime = 2.f;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsFriendly = true;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	float MovementSpeed = 50.f;

	float SpeedX;
	float SpeedY;
	float SpeedZ;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
