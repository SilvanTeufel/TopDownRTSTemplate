// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Abilitys/HoldPositionDetector.h"

#include "Characters/Unit/HoldPositionUnit.h"
#include "Characters/Unit/UnitBase.h"
// Sets default values
AHoldPositionDetector::AHoldPositionDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Laser Mesh");
	// Set the collision settings for the mesh
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	Mesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren

	// Moblility
	Mesh->SetMobility(EComponentMobility::Movable);
	
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void AHoldPositionDetector::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AHoldPositionDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
void AHoldPositionDetector::InitDetector(AActor* ShootUnit)
{
	ShootingUnit = ShootUnit;
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AHoldPositionDetector::OnDetectorOverlap);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &AHoldPositionDetector::OnDetectorEndOverlap);
}
void AHoldPositionDetector::ScaleDetectors(FVector Scaler, float Range)
{
	if(Scaler.X + Scaler.Y <= 22.f)
	{
		Scaler.X = Scaler.X + Range/250.f;
		SetActorScale3D(DetectorScaler*Scaler);
	}
}
void AHoldPositionDetector::OnDetectorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUnitBase* DetectedUnit = Cast<AUnitBase>(OtherActor);
	AHoldPositionUnit* Shooter = Cast<AHoldPositionUnit>(ShootingUnit);
	if(DetectedUnit && Shooter && Shooter->IsFriendly != DetectedUnit->IsFriendly)
		Shooter->UnitsDetected.Add(DetectedUnit);
}

void AHoldPositionDetector::OnDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AUnitBase* DetectedUnit = Cast<AUnitBase>(OtherActor);
	AHoldPositionUnit* Shooter = Cast<AHoldPositionUnit>(ShootingUnit);
	if(DetectedUnit && Shooter && Shooter->UnitsDetected.Num() && Shooter->IsFriendly != DetectedUnit->IsFriendly)
	{
		Shooter->UnitsDetected.Remove(DetectedUnit);
		
		if(DetectedUnit == Shooter->UnitToShoot)
		{
			Shooter->UnitToShoot = nullptr;
		}
	}
}

