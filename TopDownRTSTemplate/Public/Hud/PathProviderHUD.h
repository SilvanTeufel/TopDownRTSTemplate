// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Hud/HUDBase.h"
#include "Core/UnitData.h"
#include "Core/DijkstraMatrix.h"
#include "Math/Matrix.h"
#include "Engine/DataTable.h"
#include "Actors/Dijkstra/DijkstraCenter.h"
#include "Actors/Dijkstra/NoPathFindingArea.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "PathProviderHUD.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API APathProviderHUD : public AHUDBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category="TopDownRTSTemplate")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	FCollisionQueryParams QueryParams;
	
	void BeginPlay() override;
	
	void Tick(float DeltaSeconds);

	
	UPROPERTY(EditAnywhere, Category= TopDownRTSTemplate)
	uint64 MaxCosts = 99999.f;

	UPROPERTY(EditAnywhere, Category= TopDownRTSTemplate)
	float MaxDistance = 99999.f;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	bool Debug = true;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	bool StopLoading = false;
	
	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	bool DisablePathFindingOnEnemy = true;

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	bool DisablePathFindingOnFriendly = true;
	
	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	TArray<FPathMatrix> PathMatrixes;
	
	// Create the Grid for Dijkstra ///////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FoundCenterPoints", Keywords = "TopDownRTSTemplate FoundCenterPoints"), Category = TopDownRTSTemplate)
	TArray<AActor*> FoundCenterPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class UDataTable* GridDataTable;

	UPROPERTY(BlueprintReadWrite, Category= TopDownRTSTemplate)
	bool CreatedGridAndDijkstra = false;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateGridAndDijkstra", Keywords = "TopDownRTSTemplate CreateGridAndDijkstra"), Category = TopDownRTSTemplate)
	void CreateGridAndDijkstra();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateGridsfromDataTable", Keywords = "TopDownRTSTemplate CreateGridsfromDataTable"), Category = TopDownRTSTemplate)
	void CreateGridsfromDataTable();
	
	UFUNCTION(meta = (DisplayName = "CreatePathMatrix", Keywords = "TopDownRTSTemplate CreatePathMatrix"), Category = TopDownRTSTemplate)
	TArray<FPathMatrixRow> CreatePathMatrix(int ColCount, int RowCount, float Delta, FVector Offset);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////


	// Calc Dijkstra in Begin_Play ///////////////////////////////////////////////////////////////////////
	
	TArray<FDijkstraMatrix> G_DijkstraPMatrixes;

	UPROPERTY(EditAnywhere, Category= TopDownRTSTemplate)
	TArray<FPathPoint> Done;

	UFUNCTION(meta = (DisplayName = "Dijkstra", Keywords = "TopDownRTSTemplate Dijkstra"), Category = TopDownRTSTemplate)
	TArray<FDijkstraRow> Dijkstra(FVector CenterPoint, TArray<FPathMatrixRow> PMatrix);
	
	UFUNCTION(meta = (DisplayName = "DijkstraInit", Keywords = "TopDownRTSTemplate DijkstraInit"), Category = TopDownRTSTemplate)
	TArray<FDijkstraRow> DijkstraInit(TArray<FPathMatrixRow> PMatrix, TArray<FPathPoint>& Ques, int StartId, FVector3d StartPoint);

	UFUNCTION(meta = (DisplayName = "CheckDistrajkLoop", Keywords = "TopDownRTSTemplate CheckDistrajkLoop"), Category = TopDownRTSTemplate)
	bool CheckDijkstraLoop(TArray<FDijkstraRow>& DMatrix, int k, TArray<FPathMatrixRow> PMatrix, int u, TArray<FPathPoint>& NewQue,TArray<FPathPoint> DoneQue, FPathPoint CurrentQue);

	////////////////////////////////////////////////////////////////////////////////////////////////////


	// Handle Dijkstra Matrix ///////////////////////////////////////////////////////////////////////
	
	UPROPERTY(EditAnywhere, Category= TopDownRTSTemplate)
	int MaxPathIteration = 5000;
	
	UFUNCTION(meta = (DisplayName = "GetStartPoint", Keywords = "TopDownRTSTemplate GetStartPoint"), Category = TopDownRTSTemplate)
	void GetStartPoint(TArray<FPathMatrixRow> PMatrix ,int& StartId , float& ShortestDistanceToStart, FVector3d StartPoint);

	UFUNCTION(meta = (DisplayName = "GetEndPoint", Keywords = "TopDownRTSTemplate GetEndPoint"), Category = TopDownRTSTemplate)
	void GetEndPoint(TArray<FDijkstraRow> DMatrix, float& ShortestDistanceToEnd, int& EndId, FVector3d EndPoint);

	UFUNCTION(meta = (DisplayName = "CreatePathMatrix", Keywords = "TopDownRTSTemplate CreatePathMatrix"), Category = TopDownRTSTemplate)
	bool IsPointInsideArray(TArray<FPathPoint> Array, FPathPoint Point);

	UFUNCTION(meta = (DisplayName = "IsIdInsideMatrix", Keywords = "TopDownRTSTemplate IsIdInsideMatrix"), Category = TopDownRTSTemplate)
	bool IsIdInsideMatrix(TArray<FDijkstraRow> Matrix, int Id);

	UFUNCTION(meta = (DisplayName = "GivePointFromID", Keywords = "TopDownRTSTemplate GivePointFromID"), Category = TopDownRTSTemplate)
	FDijkstraRow GivePointFromID(TArray<FDijkstraRow> Matrix, int Id);

	////////////////////////////////////////////////////////////////////////////////////////////////////


	// On Real-Time ///////////////////////////////////////////////////////////////////////
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SetNextDijkstraPauseTime", Keywords = "TopDownRTSTemplate SetNextDijkstraPauseTime"), Category = TopDownRTSTemplate)
	float SetNextDijkstraPauseTime = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlTimer", Keywords = "TopDownRTSTemplate ControlTimer"), Category = TopDownRTSTemplate)
	float ControlTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StartTimer", Keywords = "TopDownRTSTemplate StartTimer"), Category = TopDownRTSTemplate)
	float StartTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StartTime", Keywords = "TopDownRTSTemplate StartTime"), Category = TopDownRTSTemplate)
	float StartTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UseDijkstraOnlyOnFirstUnit", Keywords = "TopDownRTSTemplate UseDijkstraOnlyOnFirstUnit"), Category = TopDownRTSTemplate)
	bool UseDijkstraOnlyOnFirstUnit = true;
	
	UFUNCTION(meta = (DisplayName = "GetDistrajkPath", Keywords = "TopDownRTSTemplate GetDistrajkPath"), Category = TopDownRTSTemplate)
	TArray<FPathPoint> GetDijkstraPath(TArray<FDijkstraRow>& DMatrix, int EndId);
	
	UFUNCTION(meta = (DisplayName = "GetPathReUseDijkstra", Keywords = "TopDownRTSTemplate GetPathReUseDijkstra"), Category = TopDownRTSTemplate)
	TArray<FPathPoint> GetPathReUseDijkstra(TArray<FDijkstraRow> DMatrix, FVector3d EndPoint, FVector3d StartPoint);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// SET Dijkstra on Character ///////////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNextDijkstra", Keywords = "TopDownRTSTemplate SetNextDijkstra"), Category = TopDownRTSTemplate)
	void SetNextDijkstra(TArray <AUnitBase*> Units, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNextDijkstraTo", Keywords = "TopDownRTSTemplate SetNextDijkstraTo"), Category = TopDownRTSTemplate)
	void SetNextDijkstraTo(TArray <AUnitBase*> Units, FVector Location );

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetDijkstraWithClosestZDistance", Keywords = "TopDownRTSTemplate SetDijkstraWithClosestZDistance"), Category = TopDownRTSTemplate)
	void SetDijkstraWithClosestZDistance(AUnitBase* UnitBase, float Z);////

	////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FoundCenterPoints", Keywords = "TopDownRTSTemplate FoundCenterPoints"), Category = TopDownRTSTemplate)
	TArray<ANoPathFindingArea*> NoPathFindingAreas;
	
	UPROPERTY(EditAnywhere, Category= TopDownRTSTemplate)
	float RangeThreshold = 500.f;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Location", Keywords = "TopDownRTSTemplate Location"), Category = TopDownRTSTemplate)
	bool IsLocationInNoPathFindingAreas(FVector Location);
};
