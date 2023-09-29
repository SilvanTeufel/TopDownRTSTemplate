// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Abilitys/GravityActor.h"

// Sets default values
AGravityActor::AGravityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // QueryAndPhysics
	//Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGravityActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

