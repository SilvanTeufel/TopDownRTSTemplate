// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "GameModes/UnitGeneratorGameModeBase.h"

#include "AIController.h"
#include "Controller/ControllerBase.h"

void AUnitGeneratorGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemysWithID(1, FVector(14000.f,-7500.f,0.f));
	SpawnEnemysAndWaypointsFromDataTable();
}

void AUnitGeneratorGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckSpawnTimer += DeltaTime;
	if(CheckSpawnTimer >= 10.f)
	{
		CheckSpawnTimer = 0.f;
		DespawnDeadEnemys(UnitSpawnDataSets);
		RespawnAllEnemys(UnitSpawnDataSets);
	}
}


void AUnitGeneratorGameModeBase::SpawnEnemysWithID(int id, FVector Location)
{
	if (UnitSpawnParameter)
	{
		TArray<FName> RowNames = UnitSpawnParameter->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			FUnitSpawnParameter* SpawnParameter = UnitSpawnParameter->FindRow<FUnitSpawnParameter>(RowName, TEXT(""));
			if (SpawnParameter && SpawnParameter->Id == id)
			{
			
				SpawnEnemysAndWaypoints(*SpawnParameter, Location);
			}
		}
	}
}

void AUnitGeneratorGameModeBase::SpawnEnemysAndWaypointsFromDataTable()
{
	if(!GenerateUnits) return;
	
	if (UnitSpawnParameter)
	{
		TArray<FName> RowNames = UnitSpawnParameter->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			FUnitSpawnParameter* SpawnParameter = UnitSpawnParameter->FindRow<FUnitSpawnParameter>(RowName, TEXT(""));
			if (SpawnParameter)
			{
				SpawnEnemysAndWaypoints(*SpawnParameter);
			}
		}
	}
}

void AUnitGeneratorGameModeBase::SpawnChaseEnemysWithID(int id, FVector Location, AUnitBase* UnitToChase)
{
	if (UnitSpawnParameter)
	{
		TArray<FName> RowNames = UnitSpawnParameter->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			FUnitSpawnParameter* SpawnParameter = UnitSpawnParameter->FindRow<FUnitSpawnParameter>(RowName, TEXT(""));
			if (SpawnParameter && SpawnParameter->Id == id)
			{
			
				SpawnEnemys(*SpawnParameter, Location, UnitToChase);
			}
		}
	}
}

void AUnitGeneratorGameModeBase::SpawnEnemys(FUnitSpawnParameter SpawnParameter, FVector Location, AUnitBase* UnitToChase)
{
	
	for(int i = 0; i < SpawnParameter.UnitCount; i++)
	{
		// Waypointspawn
		const FVector FirstLocation = CalcLocation(SpawnParameter.UnitOffset+Location, SpawnParameter.UnitMinRange, SpawnParameter.UnitMaxRange);
		const FVector SecondLocation = CreateSecondWaypointLocation(FirstLocation);
		FTransform FirstTransform;
		FirstTransform.SetLocation(FirstLocation);
		FirstTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		const auto FirstWayPoint = Cast<AWaypoint>
						(UGameplayStatics::BeginDeferredActorSpawnFromClass
						(this, SpawnParameter.WaypointClass, FirstTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	
		FTransform SecondTransform;
		SecondTransform.SetLocation(SecondLocation);
		SecondTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		const auto SecondWayPoint = Cast<AWaypoint>
						(UGameplayStatics::BeginDeferredActorSpawnFromClass
						(this, SpawnParameter.WaypointClass, SecondTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));


		FTransform EnemyTransform;
		 EnemyTransform.SetLocation(FVector(FirstLocation.X, FirstLocation.Y, 100.f));
		
		const auto UnitBase = Cast<AUnitBase>
			(UGameplayStatics::BeginDeferredActorSpawnFromClass
			(this, SpawnParameter.UnitBaseClass, EnemyTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

		

		AAIController* ControllerBase = GetWorld()->SpawnActor<AAIController>(SpawnParameter.UnitControllerBaseClass, FTransform());

		if(!ControllerBase) return;

		ControllerBase->Possess(UnitBase);
		
		if (UnitBase != nullptr)
		{
			FirstWayPoint->NextWaypoint = SecondWayPoint;
			SecondWayPoint->NextWaypoint = FirstWayPoint;
			UnitBase->NextWaypoint = SecondWayPoint;
			
			UE_LOG(LogTemp, Warning, TEXT("Spawned everything!"));
			// Set NextWaypoint
			UnitBase->UnitToChase = UnitToChase;
			UnitBase->SetUnitState(UnitData::Chase);

			UGameplayStatics::FinishSpawningActor(FirstWayPoint, FirstTransform);
			UGameplayStatics::FinishSpawningActor(SecondWayPoint, SecondTransform);
			UGameplayStatics::FinishSpawningActor(UnitBase, EnemyTransform);
			FUnitSpawnData UnitSpawnDataSet;
			UnitSpawnDataSet.Id = SpawnParameter.Id;
			UnitSpawnDataSet.FirstWaypoint = FirstWayPoint;
			UnitSpawnDataSet.SecondWaypoint = SecondWayPoint;
			UnitSpawnDataSet.UnitBase = UnitBase;
			UnitSpawnDataSet.SpawnParameter = SpawnParameter;
			UnitSpawnDataSets.Add(UnitSpawnDataSet);
		
		}
	}
	
	// Enemyspawn
}

void AUnitGeneratorGameModeBase::SpawnEnemysAndWaypoints(FUnitSpawnParameter SpawnParameter, FVector Location)
{
	
	for(int i = 0; i < SpawnParameter.UnitCount; i++)
	{
		// Waypointspawn
		const FVector FirstLocation = CalcLocation(SpawnParameter.UnitOffset+Location, SpawnParameter.UnitMinRange, SpawnParameter.UnitMaxRange);
		const FVector SecondLocation = CreateSecondWaypointLocation(FirstLocation);
		FTransform FirstTransform;
		FirstTransform.SetLocation(FirstLocation);
		FirstTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		const auto FirstWayPoint = Cast<AWaypoint>
						(UGameplayStatics::BeginDeferredActorSpawnFromClass
						(this, SpawnParameter.WaypointClass, FirstTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	
		FTransform SecondTransform;
		SecondTransform.SetLocation(SecondLocation);
		SecondTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		const auto SecondWayPoint = Cast<AWaypoint>
						(UGameplayStatics::BeginDeferredActorSpawnFromClass
						(this, SpawnParameter.WaypointClass, SecondTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));


		FTransform EnemyTransform;
		 EnemyTransform.SetLocation(FVector(FirstLocation.X, FirstLocation.Y, 100.f));
		
		const auto UnitBase = Cast<AUnitBase>
			(UGameplayStatics::BeginDeferredActorSpawnFromClass
			(this, SpawnParameter.UnitBaseClass, EnemyTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

		

		AAIController* ControllerBase = GetWorld()->SpawnActor<AAIController>(SpawnParameter.UnitControllerBaseClass, FTransform());

		if(!ControllerBase) return;

		ControllerBase->Possess(UnitBase);
		
		if (FirstWayPoint != nullptr && SecondWayPoint != nullptr && UnitBase != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawned everything!"));
			// Set NextWaypoint
			FirstWayPoint->NextWaypoint = SecondWayPoint;
			SecondWayPoint->NextWaypoint = FirstWayPoint;
			UnitBase->NextWaypoint = SecondWayPoint;

			UGameplayStatics::FinishSpawningActor(FirstWayPoint, FirstTransform);
			UGameplayStatics::FinishSpawningActor(SecondWayPoint, SecondTransform);
			UGameplayStatics::FinishSpawningActor(UnitBase, EnemyTransform);
			FUnitSpawnData UnitSpawnDataSet;
			UnitSpawnDataSet.Id = SpawnParameter.Id;
			UnitSpawnDataSet.FirstWaypoint = FirstWayPoint;
			UnitSpawnDataSet.SecondWaypoint = SecondWayPoint;
			UnitSpawnDataSet.UnitBase = UnitBase;
			UnitSpawnDataSet.SpawnParameter = SpawnParameter;
			UnitSpawnDataSets.Add(UnitSpawnDataSet);
		
		}
	}
	
	// Enemyspawn
}

void AUnitGeneratorGameModeBase::DespawnDeadEnemys(TArray<FUnitSpawnData> SpawnDataSets)
{	
	
	TArray<FUnitSpawnData> UnitsToRemove;
	TArray<FUnitSpawnData> NewUnitSpawnDataSets;
	
	for (FUnitSpawnData SpawnData : SpawnDataSets)
	{
		if (SpawnData.UnitBase && SpawnData.UnitBase->GetUnitState() == UnitData::Dead)
		{
			UnitsToRemove.Add(SpawnData);
		}else
		{
			NewUnitSpawnDataSets.Add(SpawnData);
		}
	}

	for (FUnitSpawnData SpawnData : UnitsToRemove)
	{

		
		if (SpawnData.UnitBase != nullptr)
		{
			SpawnData.UnitBase->Destroy(true, false);
		}
		if (SpawnData.FirstWaypoint != nullptr)
		{
			SpawnData.FirstWaypoint->Destroy(true, false);
		}
		if (SpawnData.SecondWaypoint != nullptr)
		{
			SpawnData.SecondWaypoint->Destroy(true, false);
		}
	}

	UnitSpawnDataSets = NewUnitSpawnDataSets;
	
}

void AUnitGeneratorGameModeBase::RespawnAllEnemys(TArray<FUnitSpawnData> SpawnDataSets)
{
	/*
	for(int i = 0; i < SpawnDataSets.Num(); i++)
	{
		if(SpawnDataSets.Num() <= SpawnDataSets[i].SpawnParameter.UnitCountToRespawn)
		{
			// One Enemy Dies so trigger
		
				AWaypoint* FirstWaypoint = SpawnDataSets[i].FirstWaypoint;
				AWaypoint* SecondWaypoint  = SpawnDataSets[i].SecondWaypoint;
				AUnitBase* UnitBase = SpawnDataSets[i].UnitBase;
				
				SpawnDataSets.Remove(SpawnDataSets[i]);
			
				UnitBase->Destroy(true, false);
				FirstWaypoint->Destroy(true, false);
				SecondWaypoint->Destroy(true, false);
			
				SpawnEnemysAndWaypoints(SpawnDataSets[i].SpawnParameter);
		}
	}*/
}

FVector AUnitGeneratorGameModeBase::CreateSecondWaypointLocation(FVector FirstLocation)
{
	int MultiplierA;
	const float randChooserA = FMath::RandRange(1, 10);
	if(randChooserA <= 5)
	{
		MultiplierA = -1;
	}else
	{
		MultiplierA = 1;
	}

	int MultiplierB;
	const float randChooserB = FMath::RandRange(1, 10);
	if(randChooserB <= 5)
	{
		MultiplierB = -1;
	}else
	{
		MultiplierB = 1;
	}
				
	const float RandomOffsetX = FMath::RandRange(500.f, 1000.f);
	const float RandomOffsetY = FMath::RandRange(500.f, 1000.f);
	
				
	const float X = RandomOffsetX*MultiplierA+FirstLocation.X; 
	const float Y = RandomOffsetY*MultiplierB+FirstLocation.Y; 
	const float Z = FirstLocation.Z;
				
	return FVector(X, Y, Z);
}