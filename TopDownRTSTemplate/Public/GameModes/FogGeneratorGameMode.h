// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/FogOfWar.h"
#include "GameModes/UnitGeneratorGameModeBase.h"
#include "FogGeneratorGameMode.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FFogData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector Position;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	AFogOfWar* FogOfWar;
};
UCLASS()
class TOPDOWNRTSTEMPLATE_API AFogGeneratorGameMode : public AUnitGeneratorGameModeBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	AFogGeneratorGameMode();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Generate the fog grid and spawn fog of war instances

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	int32 FogRowCount = 9;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	int32 FogColCount = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	float FogDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	FVector FogOffset = FVector(-17000.f, -6000.f, 20.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	FVector FogScale = FVector(15.f, 15.f, 10.f);


	TArray<FFogData> GenerateFogGrid(int32 RowCount, int32 ColCount, float Distance, FVector Offset, FVector Scale);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	bool GenerateFog = true;
	
	// The grid of fog data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	TArray<FFogData> FogGrid;

	// The fog of war actor class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate )
	TSubclassOf<AFogOfWar> FogOfWarClass;



	private:
	// Helper function to calculate the position of each grid cell
	FVector CalculateGridCellPosition(int32 RowIndex, int32 ColIndex, float Distance) const;
};
