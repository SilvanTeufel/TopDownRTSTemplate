// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Unit/HoldPositionController.h"
#include "Characters/Unit/ExtendedUnitBase.h"

void AHoldPositionController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if(bDisableUnitControllerBaseTick)
	//return;
	
	HoldPositionStateMachine(DeltaSeconds);
}


void AHoldPositionController::HoldPositionStateMachine(float DeltaSeconds)
{
	AHoldPositionUnit* UnitBase = Cast<AHoldPositionUnit>(GetPawn());

	if(UnitBase)
		switch (UnitBase->UnitState)
		{
		case UnitData::HoldPositionCreate:
			{
				//UE_LOG(LogTemp, Warning, TEXT("HoldPositionCreate"));
				UnitBase->SetWalkSpeed(0);
			}
			break;
		case UnitData::HoldPositionWatch:
			{
				//UE_LOG(LogTemp, Warning, TEXT("HoldPositionWatch"));
				HoldPositionWatch(UnitBase, DeltaSeconds);
			}
			break;
		case UnitData::HoldPositionPause:
			{
				//UE_LOG(LogTemp, Warning, TEXT("HoldPositionPause"));
				HoldPositionPause(UnitBase, DeltaSeconds);
			}
			break;
		case UnitData::HoldPositionAttack:
			{
				//UE_LOG(LogTemp, Warning, TEXT("HoldPositionAttack"));
				HoldPositionAttack(UnitBase, DeltaSeconds);
			}
			break;
		case UnitData::HoldPositionReload:
			{
				//UE_LOG(LogTemp, Warning, TEXT("HoldPositionReload"));
				HoldPositionReload(UnitBase, DeltaSeconds);
			}
			break;
		default:
			{
				//AHoldPositionUnit* HoldPositionUnit = Cast<AHoldPositionUnit>(UnitBase);
				if(UnitBase && UnitBase->Detector)
					UnitBase->Detector->Destroy(true);
				//UnitBase->SetUnitState(UnitData::Idle);
			}
		break;
		}
	
}

void AHoldPositionController::HoldPositionPause(AHoldPositionUnit* UnitBase, float DeltaSeconds)
{
	UnitBase->SetWalkSpeed(0);
	RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
	UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);

	AUnitBase* UnitToShoot = UnitBase->UnitToShoot;
	if(!UnitToShoot || (UnitToShoot && UnitToShoot->GetUnitState() == UnitData::Dead)) {

		if(SetNextUnitToShoot(UnitBase)) return;
				
		UnitBase->SetUnitState( UnitData::HoldPositionWatch );
				
	} else if (UnitBase->UnitControlTimer > UnitBase->PauseDuration )
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(UnitBase);
		
		if(ExtendedUnitBase && ExtendedUnitBase->MagSize[ExtendedUnitBase->ActualWeaponId] <= 0)
		{
			UnitBase->UnitControlTimer = 0.f;
			UnitBase->SetUnitState( UnitData::HoldPositionReload );
		}else if(ExtendedUnitBase)
		{
			ProjectileSpawned = false;
			UnitBase->SetUnitState(UnitData::HoldPositionAttack);
			CreateProjectile(UnitBase, UnitToShoot);	
		}

	}
}

void AHoldPositionController::HoldPositionAttack(AHoldPositionUnit* UnitBase, float DeltaSeconds)
{
	AUnitBase* UnitToShoot = UnitBase->UnitToShoot;
	
	UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);
				
	if(!UnitToShoot || (UnitToShoot && UnitToShoot->GetUnitState() == UnitData::Dead))
	{
		if(SetNextUnitToShoot(UnitBase)) return;
				
		UnitBase->SetUnitState( UnitData::HoldPositionWatch );
		return;
	}

	RotateToAttackUnit(UnitBase, UnitToShoot);
	//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT(" AttackDuration // UnitControlTimer %f // %f"),  UnitBase->AttackDuration, UnitBase->UnitControlTimer);	
				
	if (UnitBase->UnitControlTimer > (UnitBase->AttackDuration + UnitBase->PauseDuration)) {
		if(!UnitBase->UseProjectile )
		{
			// Attack without Projectile
			UnitToShoot->SetHealth(UnitToShoot->GetHealth()-UnitBase->AttackDamage);

			const TEnumAsByte<UnitData::EState> State = UnitToShoot->GetUnitState();
					
			if(State!= UnitData::Run &&
				 State != UnitData::PrepareScatterMine &&
				 State != UnitData::PrepareMouseBot &&
				 State != UnitData::TeleportStart &&
				 State!= UnitData::TeleportEnd &&
				 State != UnitData::Reload )
			{
				UnitToShoot->SetUnitState( UnitData::IsAttacked );
			}
			// Attack without Projectile	
		}
		ProjectileSpawned = false;	
		UnitBase->UnitControlTimer = 0.f;
		UnitBase->SetUnitState( UnitData::HoldPositionPause);
		
	}
}

void AHoldPositionController::HoldPositionReload(AHoldPositionUnit* UnitBase, float DeltaSeconds)
{
	AExtendedUnitBase* ExtendedUnit = Cast<AExtendedUnitBase>(UnitBase);

	if(!ExtendedUnit)
	{
		ExtendedUnit->SetUnitState(UnitData::Idle);
		return;
	}
	
	ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);
	if (ExtendedUnit->UnitControlTimer >= ExtendedUnit->ReloadTime) {
		ExtendedUnit->ReloadWeapon();
		ExtendedUnit->UnitControlTimer = 0.f;
		ExtendedUnit->SetUnitState(UnitData::HoldPositionPause);
	}
}

void AHoldPositionController::HoldPositionWatch(AHoldPositionUnit* UnitBase, float DeltaSeconds)
{
	UnitBase->SetWalkSpeed(0);
	//UnitToShoot
	//UnitsDetected
	if(SetNextUnitToShoot(UnitBase))
		UnitBase->SetUnitState(UnitData::HoldPositionPause);
}

bool AHoldPositionController::SetNextUnitToShoot(AHoldPositionUnit* UnitBase)
{
	TArray<AUnitBase*> UnitsDetected = UnitBase->UnitsDetected;
	if(!UnitsDetected.Num()) return false;
		
	float ShortestDistance = UnitBase->GetDistanceTo(UnitsDetected[0]);
	int IndexShortestDistance = 0;
	for(int i = 0; i < UnitsDetected.Num(); i++)
	{
		if(UnitsDetected[i] && UnitsDetected[i]->GetUnitState() != UnitData::Dead)
		{
			DistanceToShootingUnit = UnitBase->GetDistanceTo(UnitsDetected[i]);
						
			if(DistanceToShootingUnit < ShortestDistance)
			{
				ShortestDistance = DistanceToShootingUnit;
				IndexShortestDistance = i;
			}
		}
	}

	bool RValue = false;
	
	if(UnitsDetected[IndexShortestDistance] && UnitsDetected[IndexShortestDistance]->GetUnitState() != UnitData::Dead)
	{
		UnitBase->UnitToShoot = UnitsDetected[IndexShortestDistance];
		RValue =  true;
	}

	TArray <AUnitBase*> UnitsToDelete = UnitsDetected;
	
	for(int i = 0; i < UnitsToDelete.Num(); i++)
	{
		if(UnitsToDelete[i] && UnitsToDelete[i]->GetUnitState() == UnitData::Dead)
		{
			UnitsDetected.Remove(UnitsToDelete[i]);
		}
	}

	return RValue;
}
