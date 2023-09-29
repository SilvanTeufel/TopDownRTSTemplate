// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Compass/CompassPin.h"
#include "Actors/Compass/CompassPole.h"
#include "Components/SceneComponent.h"
// Sets default values
ACompassPin::ACompassPin()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create the pin mesh component
	PinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//SetRootComponent(PinMesh);

	// Set the collision settings for the mesh
	PinMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PinMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	PinMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// Moblility
	PinMesh->SetMobility(EComponentMobility::Movable);

	// Set the mesh to be attached to the root component
	//PinMesh->SetupAttachment(RootComponent);
	SetRootComponent(PinMesh);
}

void ACompassPin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the compass pin to face the compass pole
	if (CompassPole)
	{
		FVector Actor1Location = CompassPole->GetActorLocation();
		FVector Actor2Location = this->GetActorLocation();

		// Berechne den Vektor zwischen den beiden Positionen
		FVector Direction = Actor2Location - Actor1Location;
		FRotator ActorRotation = FRotationMatrix::MakeFromY(Direction).Rotator() + PinRotation;
		SetActorRotation(ActorRotation);
	}
}

void ACompassPin::SetCompassPole(ACompassPole* NewCompassPole)
{

	CompassPole = NewCompassPole;

}
