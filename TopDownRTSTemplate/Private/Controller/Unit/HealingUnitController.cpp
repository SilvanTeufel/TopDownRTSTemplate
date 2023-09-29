// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Controller/Unit/HealingUnitController.h"
#include "Perception/AiPerceptionComponent.h"
#include "Perception/AiSenseConfig_Sight.h"
#include "Actors/Waypoint.h"

AHealingUnitController::AHealingUnitController()
{
	PrimaryActorTick.bCanEverTick = true;
	//bDisableUnitControllerBaseTick = true;
	DetectFriendlyUnits = true;
}
/*
void AHealingUnitController::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetPerceptionComponent() != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set !"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Some Problem Occured !"));
	}
	
}

void AHealingUnitController::OnPossess(APawn* PawN)
{
	Super::OnPossess(PawN);
}*/

void AHealingUnitController::Tick(float DeltaSeconds)
{
	//Super::Tick(DeltaSeconds);
	HealingUnitControlStateMachine(DeltaSeconds);
}

void AHealingUnitController::HealingUnitControlStateMachine(float DeltaSeconds)
{
	AHealingUnit* UnitBase = Cast<AHealingUnit>(GetPawn());

	if(!UnitBase)
		return;


	switch (UnitBase->GetUnitState())
	{
	case UnitData::None:
		{
			if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("None"));
		}
		break;
	case UnitData::Dead:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Dead"));
			Dead(UnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Patrol:
		{
			//if(!UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Patrol"));
			HealPatrol(UnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Run:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Run"));
			HealRun(UnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Chase:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Chase"));
			ChaseHealTarget(UnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Healing:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Healing"));
			Healing(UnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Pause:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("HealPause"));
			HealPause(UnitBase, DeltaSeconds);
		}
		break; 

	case UnitData::IsAttacked:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("IsAttacked"));
			IsAttacked(UnitBase, DeltaSeconds);
		}
		break;

	case UnitData::Idle:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Idle"));
			//Idle(UnitBase, DeltaSeconds);
			UnitBase->SetWalkSpeed(0);

			if(UnitBase->SetNextUnitToChaseHeal())
			{
				UnitBase->SetUnitState(UnitData::Chase);
			}
		}
		break;
	default:
		{
			UnitBase->SetUnitState(UnitData::Idle);
		}
		break;
	}

	if (UnitBase->GetHealth() <= 0.f && UnitBase->GetUnitState() != UnitData::Dead) {
		KillUnitBase(UnitBase);
		UnitBase->UnitControlTimer = 0.f;
	}
	
}

void AHealingUnitController::ChaseHealTarget(AHealingUnit* UnitBase, float DeltaSeconds)
{
					if (UnitBase->UnitToChase) {
    				UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
    				
    				RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
    				DistanceToUnitToChase = GetPawn()->GetDistanceTo(UnitBase->UnitToChase);
    
    					AUnitBase* UnitToChase = UnitBase->UnitToChase;

    					if (IsUnitToChaseInRange(UnitBase)) {

    						if(!bHealActorSpawned && UnitBase->UnitControlTimer >= UnitBase->PauseDuration && UnitBase->UnitToChase->GetHealth() < UnitBase->UnitToChase->GetMaxHealth())
    						{
    							UnitBase->SpawnHealActor(UnitBase->UnitToChase);
    							UnitBase->SetUnitState(UnitData::Healing);
    							bHealActorSpawned = true;
    						}else
    						{
    							UnitBase->SetUnitState(UnitData::Pause);
    						}	
    					}
    					else {
    						FVector UnitToChaseLocation = UnitToChase->GetActorLocation();
    						
    						if(UnitToChase->IsFlying && !UnitBase->IsFlying)
    						{
    							if (DistanceToUnitToChase > UnitBase->StopRunToleranceForFlying)
    							{
    								UnitBase->SetUnitState(UnitData::Idle);
    							}
    						}
    						
    						if(UnitBase->IsFlying)
    						{
    							UnitToChaseLocation =  FVector(UnitToChaseLocation.X, UnitToChaseLocation.Y, UnitBase->FlyHeight);
    						}
    						const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitBase->GetActorLocation(), UnitToChaseLocation);
    						
    						UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);
    					}
    
    					if (DistanceToUnitToChase > LoseSightRadius) {
    						UnitBase->UnitsToChase.Remove(UnitBase->UnitToChase );
    						UnitBase->UnitToChase = nullptr;
    						
    						if(!UnitBase->IsFriendly && UnitBase->FollowPath)
    						{
    							UnitBase->RunLocationArray.Empty();
    							UnitBase->RunLocationArrayIterator = 0;
    							UnitBase->DijkstraStartPoint = UnitBase->GetActorLocation();
    							UnitBase->DijkstraEndPoint = UnitBase->NextWaypoint->GetActorLocation();
    							UnitBase->DijkstraSetPath = true;
    							UnitBase->FollowPath = false;
    							UnitBase->SetUnitState(UnitBase->UnitStatePlaceholder);
    						}else
    						{
    							UnitBase->UnitsToChase.Remove(UnitBase->UnitToChase );
    							UnitBase->UnitToChase = nullptr;
    							UnitBase->SetUnitState(UnitBase->UnitStatePlaceholder);
    						}
    						
    					}
    				}else
    				{
    					UnitBase->SetUnitState(UnitBase->UnitStatePlaceholder);
    				}
}

void AHealingUnitController::Healing(AHealingUnit* UnitBase, float DeltaSeconds)
{
	
	UnitBase->SetWalkSpeed(0);	
	RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
	UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);

	if (UnitBase->UnitControlTimer > UnitBase->AttackDuration + UnitBase->PauseDuration) {
		UnitBase->SetUnitState( UnitData::Pause );
		bHealActorSpawned = false;
		UnitBase->UnitControlTimer = 0.f;
	}
	
}

void AHealingUnitController::HealPause(AHealingUnit* UnitBase, float DeltaSeconds)
{

		UnitBase->SetWalkSpeed(0);
		RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
		UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);
	
		if(UnitBase->UnitToChase && (UnitBase->UnitToChase->GetUnitState() == UnitData::Dead || UnitBase->UnitToChase->GetHealth() == UnitBase->UnitToChase->GetMaxHealth())) {

			if(!UnitBase->SetNextUnitToChaseHeal())
			{
				UnitBase->SetUnitState( UnitData::Idle );
				return;
			}
		}


		if (UnitBase->UnitControlTimer > UnitBase->PauseDuration) {

			bHealActorSpawned = false;
			if (IsUnitToChaseInRange(UnitBase) && !bHealActorSpawned && UnitBase->SetNextUnitToChaseHeal()) {
				UnitBase->SpawnHealActor(UnitBase->UnitToChase);
				bHealActorSpawned = true;
				UnitBase->SetUnitState(UnitData::Healing);
			}else if(!IsUnitToChaseInRange(UnitBase) && UnitBase->GetUnitState() != UnitData::Run)
			{
				UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
				UnitBase->SetUnitState(UnitData::Chase);
			}
		
		}
}


void AHealingUnitController::HealRun(AHealingUnit* UnitBase, float DeltaSeconds)
{
	if(UnitBase->ToggleUnitDetection && UnitBase->UnitToChase)
	{
		if(UnitBase->SetNextUnitToChaseHeal())
		{
			UnitBase->SetUnitState(UnitData::Chase);
		}
	}
				
	UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
				
	const FVector UnitLocation = UnitBase->GetActorLocation();
		
	if(UnitBase->IsFlying)
	{
		UnitBase->RunLocation = FVector(UnitBase->RunLocation.X, UnitBase->RunLocation.Y, UnitBase->FlyHeight);
	}
	
	const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitLocation, UnitBase->RunLocation);
	UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);

	const float Distance = sqrt((UnitLocation.X-UnitBase->RunLocation.X)*(UnitLocation.X-UnitBase->RunLocation.X)+(UnitLocation.Y-UnitBase->RunLocation.Y)*(UnitLocation.Y-UnitBase->RunLocation.Y));
	
	if (Distance <= UnitBase->StopRunTolerance) {
		UnitBase->SetUnitState(UnitData::Idle);
	}
	
	UnitBase->UnitControlTimer = 0.f;
}
void AHealingUnitController::HealPatrol(AHealingUnit* UnitBase, float DeltaSeconds)
{
	UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
				
	if(UnitBase->SetNextUnitToChaseHeal())
	{
		if(IsUnitToChaseInRange(UnitBase))
		{
			UnitBase->SpawnHealActor(UnitBase->UnitToChase);
			bHealActorSpawned = true;
			UnitBase->SetUnitState(UnitData::Healing);
		}else if(!IsUnitToChaseInRange(UnitBase))
		{
			UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
			UnitBase->SetUnitState(UnitData::Chase);
		}

	} else if (UnitBase->NextWaypoint != nullptr)
	{

		FVector WaypointLocation = UnitBase->NextWaypoint->GetActorLocation();

		if(UnitBase->IsFlying)
		{
			WaypointLocation =  FVector(WaypointLocation.X, WaypointLocation.Y, UnitBase->FlyHeight);
		}

		if(UnitBase->FollowPath)
		{
			const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitBase->GetActorLocation(), UnitBase->RunLocation);
			UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);
		}else
		{
			const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitBase->GetActorLocation(), WaypointLocation);
			UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);
		}
	}
	else
	{
		UnitBase->SetUnitState(UnitData::Idle);
	}
}