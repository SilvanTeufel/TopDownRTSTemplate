// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Laser/TriggeredMovementActor.h"

// Sets default values
ATriggeredMovementActor::ATriggeredMovementActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MoverMesh = CreateDefaultSubobject<UStaticMeshComponent>("Prism Mesh");
	
	// Set the collision settings for the mesh
	MoverMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MoverMesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	MoverMesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren
	MoverMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATriggeredMovementActor::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
}

// Called every frame
void ATriggeredMovementActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DetectorsAND();
	DetectorsOR();
}

void ATriggeredMovementActor::DetectorsAND()
{
	bool DetectorsEnabled = true;
	FVector ActorLocation = GetActorLocation();
	
	for(int i = 0; i < LaserDetectorsAND.Num(); i++)
	{
		if(LaserDetectorsAND[i] && !LaserDetectorsAND[i]->Enabled)
		{
			DetectorsEnabled = false;
		}
	}

	float Distance = FVector::Distance(StartLocation, ActorLocation);
	
	if(LaserDetectorsAND.Num() && LaserDetectorsAND[0] && DetectorsEnabled)
	{
		SetActorLocation(ActorLocation+Speed);
	}else if(LaserDetectorsAND.Num() && LaserDetectorsAND[0] && Distance > DistanceTolerance)
	{
		SetActorLocation(ActorLocation-Speed);
	}
}

void ATriggeredMovementActor::DetectorsOR()
{
	FVector ActorLocation = GetActorLocation();
	float Distance = FVector::Distance(StartLocation, ActorLocation);
	bool DetectorsEnabled = false;
	for(int i = 0; i < LaserDetectorsOR.Num(); i++)
	{
		if(LaserDetectorsOR[i] && LaserDetectorsOR[i]->Enabled)
		{
			DetectorsEnabled = true;
		}
	}

	if(LaserDetectorsOR.Num() && LaserDetectorsOR[0] && DetectorsEnabled)
	{
		SetActorLocation(ActorLocation+Speed);
	}else if(LaserDetectorsOR.Num() && LaserDetectorsOR[0] && Distance > DistanceTolerance)
	{
		SetActorLocation(ActorLocation-Speed);
	}
}