// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Weapon/Projectile.h"
#include "Actors/Abilitys/Shield.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryAndPhysics
	Mesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	Mesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren
	//Mesh->SetupAttachment(RootComponent);
}

void AProjectile::Init(AActor* TargetActor, AActor* ShootingActor)
{
	Target = TargetActor;
	Shooter = ShootingActor;
	AUnitBase* ShootingUnit = Cast<AUnitBase>(ShootingActor);
	AExtendedUnitBase* ShootingUnitExtended = Cast<AExtendedUnitBase>(ShootingActor);
	if(ShootingUnit)
	{
		Damage = ShootingUnit->AttackDamage;
		IsFriendly = ShootingUnit->IsFriendly;
		MovementSpeed = ShootingUnit->ProjectileSpeed;

		if(Target)
		TargetLocation = Target->GetActorLocation();

		if(Shooter)
		ShooterLocation = Shooter->GetActorLocation();
		
		if(ShootingUnitExtended)
		{
			if(!(ShootingUnit->UnitToChase &&
				ShootingUnit->UnitToChase->IsFlying &&
				ShootingUnit->UnitToChase->GetUnitState() != UnitData::Dead) &&
				ShootingUnit->GetUnitState() != UnitData::HoldPositionAttack &&
				ShootingUnit->GetUnitState() != UnitData::HoldPositionPause &&
				ShootingUnit->GetUnitState() != UnitData::HoldPositionWatch)
			{
				TargetLocation.X = ShootingUnitExtended->MouseLocation.X;
				TargetLocation.Y = ShootingUnitExtended->MouseLocation.Y;
				TargetLocation.Z = ShootingUnitExtended->MouseLocation.Z;
			}
			
			if(ShootingUnitExtended->ProjectileMesh)
			Mesh->SetStaticMesh(ShootingUnitExtended->ProjectileMesh);
			
			if(ShootingUnitExtended->ProjectileMaterial)
			Mesh->SetMaterial(0, ShootingUnitExtended->ProjectileMaterial);
			
			//Mesh->SetMaterial(0, ShootingUnitExtended->ProjectileMaterial);

			ShootingUnitExtended->MagSize[ShootingUnitExtended->ActualWeaponId] -= 1;

			MaxPiercedTargets = ShootingUnitExtended->MaxPiercedTargets;
		}

	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime += DeltaTime;

	if(LifeTime > MaxLifeTime)
	{
		Destroy(true, false);
	}else
	{
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(ShooterLocation, TargetLocation+TargetOffset);
		AddActorLocalOffset(Direction*MovementSpeed);
	}
	
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Projectile Overlap Triggered!"));
	if(OtherActor)
	{
		AUnitBase* UnitToHit = Cast<AUnitBase>(OtherActor);
		AExtendedUnitBase* ExtendedUnitToHit = Cast<AExtendedUnitBase>(OtherActor);
		AExtendedUnitBase* ExtendedShooter =  Cast<AExtendedUnitBase>(Shooter);
		
		if(ExtendedUnitToHit && ExtendedUnitToHit->Shield && ExtendedUnitToHit->Shield->IsFriendly != IsFriendly)
		{
			AShield* Shield = ExtendedUnitToHit->Shield;
			
			if(Shield && Shield->IsActive && Shield->GetHitCounter() >= 1)
			{
				Shield->HitCounter--;
				
				if(Shield->HitCounter == 0)
					ExtendedUnitToHit->ToggleShield();
				
				Destroy(true, false);
			}
			else
			{
				float DamageCreated = CreateDamageWithRange(ExtendedShooter, UnitToHit);
				UnitToHit->SetHealth(UnitToHit->GetHealth()-DamageCreated);
				//UnitToHit->SpawnDamageIndicator();
				
				const TEnumAsByte<UnitData::EState> State = ExtendedUnitToHit->GetUnitState();
				
				if(State != UnitData::Run &&
				State != UnitData::PrepareScatterMine &&
				State != UnitData::PrepareMouseBot &&
				State != UnitData::TeleportStart &&
				State != UnitData::TeleportEnd &&
				State != UnitData::Reload &&
				UnitToHit->GetUnitState() != UnitData::IsAttacked)
				{
					UnitToHit->SetUnitState( UnitData::IsAttacked );
					UnitToHit->UnitControlTimer = 0.f;
				}

				if(ExtendedShooter)
				{
					ExtendedShooter->AddEnergy(EnergyToAdd);
					ExtendedShooter->SpawnIndicator(DamageCreated, UnitToHit->GetActorLocation());
				}

				PiercedTargets += 1;
				if(PiercedTargets >= MaxPiercedTargets)
					Destroy(true, false);
				
			}
		}else if(UnitToHit && UnitToHit->IsFriendly != IsFriendly)
		{
			float DamageCreated = CreateDamageWithRange(ExtendedShooter, UnitToHit);
			
			
			UnitToHit->SetHealth(UnitToHit->GetHealth()-DamageCreated);
			
			//UnitToHit->SpawnDamageIndicator();
			
			const TEnumAsByte<UnitData::EState> State = UnitToHit->GetUnitState();
			
			if(State != UnitData::Run &&
				State != UnitData::IsAttacked)
			{
				UnitToHit->SetUnitState( UnitData::IsAttacked );
				UnitToHit->UnitControlTimer = 0.f;
			}
			
			if(ExtendedShooter)
			{
				ExtendedShooter->AddEnergy(EnergyToAdd);
				ExtendedShooter->SpawnIndicator(DamageCreated, UnitToHit->GetActorLocation());
			}
			PiercedTargets += 1;
			if(PiercedTargets >= MaxPiercedTargets)
				Destroy(true, false);
		}


		
	}
}

float AProjectile::CreateDamageWithRange(AExtendedUnitBase* ExtendedShooter, AUnitBase* UnitToHit)
{
	float RangeDamping = 0.f;
	if(ExtendedShooter)
	{
		float RealRange = (ShooterLocation-UnitToHit->GetActorLocation()).Size();
		RangeDamping =  RealRange-ExtendedShooter->Range;
	}
			
	float RealDamage = Damage-RangeDamping*RangeDampScaler;
	RealDamage = RealDamage > 0.f ? RealDamage : 0.f;
	RealDamage = RealDamage > Damage ? RealDamage : Damage;
	
	return RealDamage;
}