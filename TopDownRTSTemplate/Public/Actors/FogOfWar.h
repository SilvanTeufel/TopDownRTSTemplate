// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "FogOfWar.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AFogOfWar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogOfWar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void InitOverlaps();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UStaticMeshComponent* FogMesh;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void DisableFog();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void EnableFog();
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void OnDetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetFogVisibility(bool bVisible);

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = TopDownRTSTemplate)
	TArray<AFogExplorer*> ExplorersGetHidden ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = TopDownRTSTemplate)
	TArray<AFogExplorer*>  ExplorersRevealedFog;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = TopDownRTSTemplate)
	//class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = TopDownRTSTemplate)
	bool Visible = true;
	

};
