// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Compass/CompassPole.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
// Sets default values
ACompassPole::ACompassPole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the pin mesh component
	PoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//SetRootComponent(PoleMesh);

	// Set the collision settings for the mesh
	PoleMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PoleMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	PoleMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// Moblility
	PoleMesh->SetMobility(EComponentMobility::Movable);
	
	// Set the mesh to be attached to the root component
	//PoleMesh->SetupAttachment(RootComponent);
	SetRootComponent(PoleMesh);
}

// Called when the game starts or when spawned
void ACompassPole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACompassPole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Timer = Timer + DeltaTime;
	if(PoleIsMoving && Timer <= 10.f)
	{
		AddActorLocalOffset(Direction*MovementSpeed);
	}else if(PoleIsMoving && Timer <= 20.f)
	{
		AddActorLocalOffset(Direction*MovementSpeed*(-1));
	}else
	{
		Timer = 0.f;
	}
}

