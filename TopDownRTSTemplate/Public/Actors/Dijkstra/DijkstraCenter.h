// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DijkstraCenter.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ADijkstraCenter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADijkstraCenter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UMaterialInterface* Material;
	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
