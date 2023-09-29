// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/ComponentGeneratorGameModeBase.h"
#include "Controller/ControllerBase.h"
#include "UnitGeneratorGameModeBase.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FUnitSpawnParameter : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TSubclassOf<class AWaypoint> WaypointClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TSubclassOf<class AUnitBase> UnitBaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TSubclassOf<class AAIController> UnitControllerBaseClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int UnitCount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int UnitCountToRespawn = 10 ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector UnitOffset = FVector(3000.f,3000.f,1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector UnitMinRange = FVector(0.f,0.f,0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector UnitMaxRange = FVector(2000.f,2000.f,0.f);
};

USTRUCT(BlueprintType)
struct FUnitSpawnData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class AWaypoint* FirstWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class AWaypoint* SecondWaypoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class AUnitBase* UnitBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FUnitSpawnParameter SpawnParameter;
};



UCLASS()
class TOPDOWNRTSTEMPLATE_API AUnitGeneratorGameModeBase : public AComponentGeneratorGameModeBase
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnEnemysWithID(int id, FVector Location = FVector(0.f,0.f,0.f));

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnChaseEnemysWithID(int id, FVector Location, AUnitBase* UnitToChase);
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnEnemys(FUnitSpawnParameter SpawnParameter, FVector Location, AUnitBase* UnitToChase);
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnEnemysAndWaypoints(FUnitSpawnParameter SpawnParameter, FVector Location = FVector(0.f,0.f,0.f));

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnEnemysAndWaypointsFromDataTable();
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void DespawnDeadEnemys(TArray<FUnitSpawnData> SpawnDataSets);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void RespawnAllEnemys(TArray<FUnitSpawnData> SpawnDataSets);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	FVector CreateSecondWaypointLocation(FVector FirstLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CheckSpawnTimer)
	float CheckSpawnTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TArray<FUnitSpawnData> UnitSpawnDataSets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class UDataTable* UnitSpawnParameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool GenerateUnits = false;

};
