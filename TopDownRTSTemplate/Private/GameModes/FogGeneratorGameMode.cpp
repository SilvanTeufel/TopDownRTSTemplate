// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "GameModes/FogGeneratorGameMode.h"


// Sets default values for this component's properties
AFogGeneratorGameMode::AFogGeneratorGameMode()
{
	
}

// Called when the game starts
void AFogGeneratorGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	FogGrid = GenerateFogGrid(FogRowCount, FogColCount, FogDistance, FogOffset, FogScale);
}

// Generate the fog grid and spawn fog of war instances
TArray<FFogData> AFogGeneratorGameMode::GenerateFogGrid(int32 RowCount, int32 ColCount, float Distance, FVector Offset, FVector Scale)
{
	TArray<FFogData> Grid;

	if(!GenerateFog) return Grid;
	
	for (int32 RowIndex = 0; RowIndex < RowCount; ++RowIndex)
	{
		for (int32 ColIndex = 0; ColIndex < ColCount; ++ColIndex)
		{
			FFogData FogData;
			FogData.Id = RowIndex * ColCount + ColIndex;
			FogData.Position = CalculateGridCellPosition(RowIndex, ColIndex, Distance)+Offset;
			AFogOfWar* FogOfWar = GetWorld()->SpawnActor<AFogOfWar>(FogOfWarClass, FogData.Position, FRotator::ZeroRotator);

				if(FogOfWar)
				{
					// Add random rotation
					FRotator RandomRotation(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f);
					FogOfWar->SetActorRotation(RandomRotation);
					FogOfWar->InitOverlaps();
					FogOfWar->SetActorScale3D(Scale);
					FogData.FogOfWar = FogOfWar;
				}
			Grid.Add(FogData);
		}
	}

	return Grid;
}

// Helper function to calculate the position of each grid cell
FVector AFogGeneratorGameMode::CalculateGridCellPosition(int32 RowIndex, int32 ColIndex, float Distance) const
{
	// Adjust the calculation as per your requirements
	FVector GridCellPosition(RowIndex * Distance, ColIndex * Distance, 0.0f);
	return GridCellPosition;
}
