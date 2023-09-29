// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Controller/Unit/ExtendedUnitControllerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Actors/Weapon/WeaponBase.h"
#include "Characters/Unit/FogExplorer.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Kismet/KismetMathLibrary.h"

AExtendedUnitControllerBase::AExtendedUnitControllerBase(): Super()
{
}

void AExtendedUnitControllerBase::BeginPlay()
{
	Super::BeginPlay();
}



void AExtendedUnitControllerBase::Tick(float DeltaSeconds)
{
	ExtendedUnitControlMachine(DeltaSeconds);
	HoldPositionStateMachine(DeltaSeconds);
	/*
	AUnitBase* CurrentUnit = Cast<AUnitBase>(GetPawn());
	
	AActor* HitActor = PerformLineTrace(CurrentUnit, CurrentUnit->UnitsToChase);*/
}



void AExtendedUnitControllerBase::ExtendedUnitControlMachine(float DeltaSeconds)
{

	AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(GetPawn());

	if (ExtendedUnitBase->GetHealth() <= 0.f && ExtendedUnitBase->GetUnitState() != UnitData::Dead) {
		KillUnitBase(ExtendedUnitBase);
		ExtendedUnitBase->UnitControlTimer = 0.f;
	}
	
	if(ExtendedUnitBase)
	{
		if(ExtendedUnitBase->JumpCounter >= 0.f)
		{
			ExtendedUnitBase->JumpCounterTime = (ExtendedUnitBase->JumpCounterTime + DeltaSeconds);
			if(ExtendedUnitBase->JumpCounterTime >= ExtendedUnitBase->JumpCounterResetTime)
			{
				ExtendedUnitBase->JumpCounterTime = 0.f;
				ExtendedUnitBase->JumpCounter = 0.f;
			}	
		}
	}
	//  Jump Counter Timer
	
	if(ExtendedUnitBase)
	switch (ExtendedUnitBase->UnitState)
	{
	case UnitData::ThrowWeapon:
		{
			if(ExtendedUnitBase && ExtendedUnitBase->UnitToThrowWeapon && ExtendedUnitBase->Weapon)
			{
				//UE_LOG(LogTemp, Warning, TEXT("ThrowWeapon"));
				ThrowWeapon(ExtendedUnitBase, DeltaSeconds);
			}
		}
		break;
	case UnitData::WeaponGravity:
		{
			WeaponGravity(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::UseGravity:
		{
			UseGravity(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Reload:
		{
			Reload(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::TeleportStart:
		{
			TeleportStart(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::TeleportEnd:
		{
			TeleportEnd(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::PrepareScatterMine:
		{
			PrepareScatterMine(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::PrepareMouseBot:
		{
			PrepareMouseBot(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::NoMana:
		{
			NoMana(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Jump:
		{
			Jump(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Charge:
		{
			Charge(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	
	case UnitData::None:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("None"));
		}
		break;
	case UnitData::Dead:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Dead"));
			Dead(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Patrol:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Patrol"));
			Patrol(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Run:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Run"));
			Run(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Chase:
		{
			//UE_LOG(LogTemp, Warning, TEXT("Chase"));
			//if(!UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Chase"));
			ExtendedChase(ExtendedUnitBase, DeltaSeconds);
			//SetBackToIdleAndCreateRadius(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Attack:
		{
			ExtendedAttack(ExtendedUnitBase, DeltaSeconds);
			//SetBackToIdleAndCreateRadius(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	case UnitData::Pause:
		{
			//if(!UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Pause"));
			ExtendedPause(ExtendedUnitBase, DeltaSeconds);
		}
		break;

	case UnitData::IsAttacked:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("IsAttacked"));
			IsAttacked(ExtendedUnitBase, DeltaSeconds);

		}
		break;

	case UnitData::Idle:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Idle"));
			Idle(ExtendedUnitBase, DeltaSeconds);
		}
		break;
	default:
		{
			/*
			if(ExtendedUnitBase->ToggleUnitDetection)
				ExtendedUnitBase->CreateIconActionRadius();
			else
				ExtendedUnitBase->CreateIconNormalRadius();
			*/
		}
		break;
	}	
	
}



void AExtendedUnitControllerBase::Charge(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);

	if(ExtendedUnit->UnitControlTimer > ExtendedUnit->ChargeTime)
	{
		ExtendedUnit->SetUnitState(UnitData::Idle);
	}
}

void AExtendedUnitControllerBase::Jump(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);

	if(ExtendedUnit->UnitControlTimer > ExtendedUnit->JumpTime){
		ExtendedUnit->UnitControlTimer = 0;
		ExtendedUnit->SetUnitState(UnitData::Idle);
	}
}

void AExtendedUnitControllerBase::NoMana(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);
	if(ExtendedUnit->UnitControlTimer > ExtendedUnit->NoManaTime){
		ExtendedUnit->UnitControlTimer = 0;
		ExtendedUnit->SetUnitState(UnitData::Idle);
	}
}

void AExtendedUnitControllerBase::PrepareMouseBot(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	if(ExtendedUnit->Mana < ExtendedUnit->BotManaCost)
	{
		ExtendedUnit->UnitControlTimer = 0;
		ExtendedUnit->SetUnitState(UnitData::NoMana);
		return;
	}
			
	ExtendedUnit->CreateMouseBotSpawnRadius();
	ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);
			
	if (ExtendedUnit->UnitControlTimer >= ExtendedUnit->MouseBotCastTime)
	{
		FVector ALocation = ExtendedUnit->GetActorLocation();
				
		float vdist = FVector::Dist(ALocation, ExtendedUnit->MouseBotSpawnLocation) / 35;
				
		if(vdist > ExtendedUnit->MouseBotRadius)
			ExtendedUnit->MouseBotSpawnLocation = FVector(ALocation.X, ALocation.Y, ExtendedUnit->MouseBotSpawnLocation.Z);


		ExtendedUnit->MouseBotSpawnLocation = FVector( ExtendedUnit->MouseBotSpawnLocation.X, ExtendedUnit->MouseBotSpawnLocation.Y, ExtendedUnit->MouseBotSpawnLocation.Z+500.f);
				
		ExtendedUnit->SpawnMouseBot();
		ExtendedUnit->UnitControlTimer = 0.f;
		ExtendedUnit->SetUnitState(UnitData::Idle);
	}
}

void AExtendedUnitControllerBase::PrepareScatterMine(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
		if(ExtendedUnit->Mana < ExtendedUnit->MineManaCost)
		{
			ExtendedUnit->UnitControlTimer = 0;
			ExtendedUnit->SetUnitState(UnitData::NoMana);
			return;
		}

			
		ExtendedUnit->CreateMineSpawnRadius();
		ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);
		if (ExtendedUnit->UnitControlTimer >= ExtendedUnit->ScatterMineCastTime)
		{
			FVector ALocation = ExtendedUnit->GetActorLocation();
				
			float vdist = FVector::Dist(ALocation, ExtendedUnit->ScatterMineSpawnLocation) / 35;
				
				
			if(vdist > ExtendedUnit->MineSpawnRadius)
				ExtendedUnit->ScatterMineSpawnLocation = FVector(ALocation.X, ALocation.Y, ExtendedUnit->ScatterMineSpawnLocation.Z);
				
			ExtendedUnit->SpawnScatterMine(ExtendedUnit->UnitToChase, ExtendedUnit);
			ExtendedUnit->UnitControlTimer = 0.f;
			ExtendedUnit->SetUnitState(UnitData::Idle);
		}
}

void AExtendedUnitControllerBase::TeleportEnd(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds){
		ExtendedUnit->CreateIconNormalRadius();
		ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);
		if (ExtendedUnit->UnitControlTimer >= ExtendedUnit->TeleportStartTime)
		{
			ExtendedUnit->UnitControlTimer = 0.f;
			ExtendedUnit->SetUnitState(UnitData::Idle);
		}
}

void AExtendedUnitControllerBase::TeleportStart(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
		if(ExtendedUnit->Mana < ExtendedUnit->TeleportManaCost)
		{
			ExtendedUnit->UnitControlTimer = 0;
			ExtendedUnit->SetUnitState(UnitData::NoMana);
			return;
		}
			
		ExtendedUnit->CreateTeleportRadius();
		ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);
		if (ExtendedUnit->UnitControlTimer >= ExtendedUnit->TeleportStartTime)
		{

			float vdist = FVector::Dist(ExtendedUnit->GetActorLocation(), ExtendedUnit->TeleportLocation) / 35;

			if (ExtendedUnit->TeleportRadius < vdist) { // Out of Range, you Teleport into yourself
				ExtendedUnit->TeleportLocation = ExtendedUnit->GetActorLocation();
				ExtendedUnit->SetHealth(ExtendedUnit->GetHealth() -ExtendedUnit->TeleportFailedDmg);
			}

				
			const FRotator NewRotation = ExtendedUnit->GetActorRotation();
			ExtendedUnit->SetActorLocationAndRotation(ExtendedUnit->TeleportLocation, NewRotation, false, 0, ETeleportType::None);
			ExtendedUnit->UnitControlTimer = 0.f;
			ExtendedUnit->SetUnitState(UnitData::TeleportEnd);
			ExtendedUnit->Mana -= ExtendedUnit->TeleportManaCost;
		}
}

void AExtendedUnitControllerBase::SetBackToIdleAndCreateRadius(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	if(!ExtendedUnit->ToggleUnitDetection)
	{
		ExtendedUnit->SetUnitState(UnitData::Idle);
		return;
	}

	if(ExtendedUnit->ToggleUnitDetection)
		ExtendedUnit->CreateIconActionRadius();
	else
		ExtendedUnit->CreateIconNormalRadius();
}

void AExtendedUnitControllerBase::Reload(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);
	if (ExtendedUnit->UnitControlTimer >= ExtendedUnit->ReloadTime) {
		ExtendedUnit->ReloadWeapon();
		ExtendedUnit->UnitControlTimer = 0.f;
		ExtendedUnit->SetUnitState(UnitData::Idle);
	}
}

void AExtendedUnitControllerBase::UseGravity(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
				ExtendedUnit->UnitControlTimer = (ExtendedUnit->UnitControlTimer + DeltaSeconds);

			if(ExtendedUnit->GravityActor)
			{
				FVector GravityActorLocation = ExtendedUnit->GravityActor->GetActorLocation();
	
				if(abs(ExtendedUnit->MouseLocation.X - GravityActorLocation.X) > 100.f ||  abs(ExtendedUnit->MouseLocation.Y - GravityActorLocation.Y) > 100.f)
				{
					ExtendedUnit->GravityActor->Mesh->SetSimulatePhysics(true);
					FVector DirectionVector = UKismetMathLibrary::GetDirectionUnitVector(GravityActorLocation,FVector(ExtendedUnit->MouseLocation.X, ExtendedUnit->MouseLocation.Y, GravityActorLocation.Z ));

					if(GravityActorLocation.Z > ExtendedUnit->GetActorLocation().Z)
					DirectionVector = FVector(DirectionVector.X, DirectionVector.Y, 0.025f);
					
					ExtendedUnit->GravityActor->SetActorLocation(GravityActorLocation+DirectionVector*ExtendedUnit->GravityScaler);

					FRotator CurrentRotation = ExtendedUnit->GravityActor->GetActorRotation();
					
					if(CurrentRotation.Pitch > 10.f) GravityRotation = FRotator( -0.5f, 0.f, 0.f);	
					else if(CurrentRotation.Pitch < -10.f) GravityRotation = FRotator( 0.5f, 0.f, 0.f);

					ExtendedUnit->GravityActor->SetActorRotation(CurrentRotation + GravityRotation);
					
				} else
				{
					ExtendedUnit->GravityActor->Mesh->SetSimulatePhysics(false);
					if(GravityActorLocation.Z < ExtendedUnit->GetActorLocation().Z + 250.f)
					{
						FVector DirectionVector = FVector(0.f,0.f,1.f);
						ExtendedUnit->GravityActor->SetActorLocation(GravityActorLocation+DirectionVector*ExtendedUnit->GravityScaler*0.05);	
					}
					
					FRotator CurrentRotation = ExtendedUnit->GravityActor->GetActorRotation();
					
					if(CurrentRotation.Pitch > 10.f) GravityRotation = FRotator( -0.5f, 0.f, 0.f);	
					else if(CurrentRotation.Pitch < -10.f) GravityRotation = FRotator( 0.5f, 0.f, 0.f);

					ExtendedUnit->GravityActor->SetActorRotation(CurrentRotation + GravityRotation);
				}
			}
}

void AExtendedUnitControllerBase::WeaponGravity(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	ExtendedUnit->UnitControlTimer+= DeltaSeconds;
	if(ExtendedUnit && ExtendedUnit->UnitToThrowWeapon && ExtendedUnit->Weapon)
	{
		//UE_LOG(LogTemp, Warning, TEXT("WeaponGravity"));
		if(ExtendedUnit->UnitControlTimer > WeaponGravityTime)
		{
			ExtendedUnit->UnitControlTimer = 0;
			ExtendedUnit->SetUnitState(UnitData::Idle);
		}
	}
}

void AExtendedUnitControllerBase::ThrowWeapon(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds) {
	if(ExtendedUnit && ExtendedUnit->UnitToThrowWeapon && ExtendedUnit->Weapon)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ThrowWeapon"));
	
		ExtendedUnit->UnitControlTimer+= DeltaSeconds;
		if(ExtendedUnit->UnitControlTimer > WeaponThrowTime)
		{
			ExtendedUnit->UnitControlTimer = 0;
			ExtendedUnit->SetUnitState(UnitData::Idle);
		}
	}
}


void AExtendedUnitControllerBase::ExtendedAttack(AExtendedUnitBase* ExtendedUnitBase, float DeltaSeconds)
{
	//RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
				
	if(!ExtendedUnitBase)
		return;

	ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
	
	if(ExtendedUnitBase->UnitToChase && ExtendedUnitBase->UnitToChase->GetUnitState() == UnitData::Dead)
	{
		ExtendedUnitBase->SetNextUnitToChase();
	}
	
	if (ExtendedUnitBase->UnitControlTimer > (ExtendedUnitBase->AttackDuration + ExtendedUnitBase->PauseDuration)) {
		ProjectileSpawned = false;	
		ExtendedUnitBase->UnitControlTimer = 0.f;
		ExtendedUnitBase->SetUnitState( UnitData::Pause);
				
	}
}


void AExtendedUnitControllerBase::ExtendedChase(AExtendedUnitBase* ExtendedUnitBase, float DeltaSeconds)
{
				if(!ExtendedUnitBase)
				return;
	
				ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
				
			
				ExtendedUnitBase->SetWalkSpeed(ExtendedUnitBase->MaxRunSpeed);
					
				//RotateToAttackUnit(ExtendedUnitBase, ExtendedUnitBase->UnitToChase);
				//DistanceToUnitToChase = GetPawn()->GetDistanceTo(ExtendedUnitBase->UnitToChase);
						if(ExtendedUnitBase && ExtendedUnitBase->UnitControlTimer >= ExtendedUnitBase->PauseDuration)
						{
								// Check if there is still something inside the Magazine
								if(ExtendedUnitBase->MagSize[ExtendedUnitBase->ActualWeaponId] > 0 && !ExtendedUnitBase->UnitDidThrowWeapon)
								{
									// Decrease Magsize by 1 and Attack
									ExtendedUnitBase->SetUnitState(UnitData::Attack);
									CreateProjectile(ExtendedUnitBase, ExtendedUnitBase->UnitToChase);
									return;
								}
								ExtendedUnitBase->SetUnitState(UnitData::Idle);
							
						}else
						{
							ExtendedUnitBase->SetUnitState(UnitData::Pause);
						}
				
}


void AExtendedUnitControllerBase::ExtendedPause(AExtendedUnitBase* ExtendedUnitBase, float DeltaSeconds)
{
	if(!ExtendedUnitBase)
		return;
	
	ExtendedUnitBase->SetWalkSpeed(0);
	ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
	
	if(ExtendedUnitBase->UnitToChase && ExtendedUnitBase->UnitToChase->GetUnitState() == UnitData::Dead)
	{
		ExtendedUnitBase->SetNextUnitToChase();
	}

	if (ExtendedUnitBase->UnitControlTimer > ExtendedUnitBase->PauseDuration ) {
		if(ExtendedUnitBase && (ExtendedUnitBase->MagSize[ExtendedUnitBase->ActualWeaponId] <= 0 || ExtendedUnitBase->UnitDidThrowWeapon))
		{
			ExtendedUnitBase->SetUnitState( UnitData::Idle );
			return;
		}
		
		ProjectileSpawned = false;
		CreateProjectile(ExtendedUnitBase, ExtendedUnitBase->UnitToChase);
		ExtendedUnitBase->SetUnitState( UnitData::Attack );
		
	}
}
