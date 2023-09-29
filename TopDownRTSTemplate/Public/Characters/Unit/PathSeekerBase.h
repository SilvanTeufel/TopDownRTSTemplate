// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
//#include "UnitBase.h"
#include "GameFramework/Character.h"
#include "Core/UnitData.h"
#include "Actors/Dijkstra/DijkstraCenter.h"
#include "Core/DijkstraMatrix.h"
#include "PathSeekerBase.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API APathSeekerBase : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector DijkstraStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector  DijkstraEndPoint;

	UPROPERTY()
	FDijkstraMatrix Next_DijkstraPMatrixes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool  DijkstraSetPath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool  FollowPath = false;

};
