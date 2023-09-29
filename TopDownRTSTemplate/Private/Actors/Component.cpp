// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Actors/Component.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
// Sets default values
AComponent::AComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ComponentMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
}

// Called when the game starts or when spawned
void AComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentMovementTime += DeltaTime;
	if(!ComponentName.IsEmpty() && IsMoving && CurrentMovementTime > 0.05f)
	{
		const FVector Location = GetActorLocation();
		const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), MovementSpeed);

		if(CurrentMovementRange <= MovementRange && !DirectionChange)
		{
			const FVector NewLocation =Location+MovementSpeed*2;
			SetActorLocation(NewLocation);
			CurrentMovementRange += FVector::Dist(Location, NewLocation);
		}else
		{
			DirectionChange = true;
			const FVector NewLocation =Location-MovementSpeed*2;
			SetActorLocation(NewLocation);
			CurrentMovementRange -= FVector::Dist(Location, NewLocation);
			if(CurrentMovementRange <= 0.f) DirectionChange = false;
		}

		
	}
	
}

void AComponent::Init(FString Name, class UStaticMesh* Mesh, UMaterial* Material, bool BIsMoving, float FMovementRange, FVector VMovementSpeed) //, class UStaticMeshComponent* MeshComp)
{
	
	ComponentName = Name;
	IsMoving = BIsMoving;
	MovementRange = FMovementRange;
	MovementSpeed = VMovementSpeed;
	ComponentMesh->SetStaticMesh(Mesh); 
	ComponentMesh->SetMaterial(0, Material);

}

