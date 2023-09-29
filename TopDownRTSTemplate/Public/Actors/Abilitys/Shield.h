// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "Shield.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AShield : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShield();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		UStaticMeshComponent* SphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		UMaterialInterface* BlueMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		UMaterialInterface* HiddenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int HitCounter = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int MaxHitCounter = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float HitCounterRefreshTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float HitCounterMaxRefreshTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsFriendly = true;
	//UMaterialInterface* Material;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void Hide();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void Show();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void RefreshCounterTick(float DeltaTime);

	int GetHitCounter() { 
		return HitCounter;
	};
	
	int GetMaxHitCounter() {
		return MaxHitCounter;
	};
};

