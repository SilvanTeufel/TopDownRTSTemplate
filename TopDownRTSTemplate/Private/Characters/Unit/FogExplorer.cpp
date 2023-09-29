// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Unit/FogExplorer.h"
#include "Perception/AIPerceptionComponent.h"
#include "Actors/FogOfWar.h"

void AFogExplorer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AFogExplorer::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsVisibleInFog)
	SpawnFogDetector();
}

void AFogExplorer::DetectActorsInRadius(float Radius)
{
	TArray<FHitResult> HitResults;
	FVector TraceStart = GetActorLocation(); // Starting point of the trace
	FVector TraceEnd = TraceStart; // The trace end location is the same as the start since it's a sphere
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Radius);

	// Perform the sphere trace
	FCollisionQueryParams QueryParams(FName(TEXT("DetectActorsInRadius")), true, this);
	GetWorld()->SweepMultiByChannel(HitResults, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, CollisionShape, QueryParams);

	// Loop through the hit results
	for (const FHitResult& HitResult : HitResults)
	{
		// Check if the hit actor is valid
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor != this)
		{
			AFogOfWar* FogOfWar = Cast<AFogOfWar>(HitActor);

			if(FogOfWar)
				FogOfWar->Visible = false;
			
		}
		
		
	}

	// Optional: Draw a debug sphere in the editor to visualize the detection radius
	DrawDebugSphere(GetWorld(), TraceStart, Radius, 16, FColor::Green, false, 2.0f);
}

void AFogExplorer::SpawnFogDetector()
{
	if (!FogDetectorClass)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform DetectorTransform;
	DetectorTransform.SetLocation(FVector(0, 0, 0));
	DetectorTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	AFogDetector* FogDetector = GetWorld()->SpawnActor<AFogDetector>(FogDetectorClass, DetectorTransform, SpawnParams);

	if (FogDetector)
	{
		USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
		FogDetector->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("rootSocket"));
		FogDetector->SetActorScale3D(FVector(80.0f));
		FogDetector->SetActorRelativeLocation(FVector(0, 0, 70));
		FogDetector->SetDetectorOwner(this);
		FogDetector->InitOverlaps();
	}
}