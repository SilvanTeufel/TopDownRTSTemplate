// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Laser/LaserBeam.h"
#include "Characters/Unit/UnitBase.h"

// Sets default values
ALaserBeam::ALaserBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>("Laser Mesh");
	// Set the collision settings for the mesh
	LaserMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LaserMesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	LaserMesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren

	// Moblility
	LaserMesh->SetMobility(EComponentMobility::Movable);
	
	RootComponent = LaserMesh;
}

// Called when the game starts or when spawned
void ALaserBeam::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserBeam::ChangeLaserMaterial()
{
	LaserMaterial = UMaterialInstanceDynamic::Create(LaserMesh->GetMaterial(0), this);

	if (LaserMaterial)
	{
		LaserMesh->SetMaterial(0, LaserMaterial);
	}
}

void ALaserBeam::KillAfterLifeTime(float DeltaTime)
{
	LifeTime += DeltaTime;

	if(LifeTime >= MaxLifeTime)
	{
		Destroy(true);
	}
}

// Called every frame
void ALaserBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateUnits(DeltaTime);
	MoveLaser(DeltaTime);
	KillAfterLifeTime(DeltaTime);
}

void ALaserBeam::AddUnit(AUnitBase* Unit)
{
	Unit->SetHealth(Unit->GetHealth()-LaserDamage);
	const TEnumAsByte<UnitData::EState> State = Unit->GetUnitState();
				
	if(State != UnitData::Run &&
			State != UnitData::IsAttacked)
	{
		Unit->SetUnitState( UnitData::IsAttacked );
		Unit->UnitControlTimer = 0.f;
	}
	
	UnitBases.Add(Unit);
};

void ALaserBeam::RemoveUnit(AUnitBase* Unit)
{
	UnitBases.Remove(Unit);
};

void ALaserBeam::UpdateUnits(float DeltaTime)
{
	ControlTime += DeltaTime;
	
	if (ControlTime >= UpdateInterval)
	{
		for (AUnitBase* Unit : UnitBases)
		{
			if (Unit != nullptr)
			{
				Unit->SetHealth(Unit->GetHealth()-LaserIntervalDamage);
				const TEnumAsByte<UnitData::EState> State = Unit->GetUnitState();
				
				if(State != UnitData::Run &&
						State != UnitData::IsAttacked)
				{
					Unit->SetUnitState( UnitData::IsAttacked );
					Unit->UnitControlTimer = 0.f;
				}
			}
		}
		ControlTime = 0.f;
	}
}

void ALaserBeam::MoveLaser(float DeltaTime)
{
	ControlTime += DeltaTime;


	const FVector Add = DirectionVector*300.f;
	const FVector ActorLocation = GetActorLocation();
	SetActorLocation(ActorLocation + Add);

	//SetActorRelativeLocation(Add);
}

void ALaserBeam::OnLaserOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
			AUnitBase* Unit = Cast<AUnitBase>(OtherActor);
	
			if (Unit)
			{
				AddUnit(Unit);
			}


}

void ALaserBeam::OnLaserEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AUnitBase* Unit = Cast<AUnitBase>(OtherActor);
			
	if (Unit )
	{
				RemoveUnit(Unit);
	}

}
