// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/FogDetector.h"
#include "Actors/FogOfWar.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "CollisionQueryParams.h"

// Sets default values
AFogDetector::AFogDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(DetectorMesh);
	DetectorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryAndPhysics
	DetectorMesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	DetectorMesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren

}

void AFogDetector::OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AFogExplorer* Explorer = Cast<AFogExplorer>(OtherActor);
	if(Explorer && !Explorer->IsVisibleInFog && DetectorOwner)
	{


		//Explorer->SetActorHiddenInGame(false);
		
		DetectUnits.Add(Explorer);
		// Perform line trace from DetectorOwner to Explorer
		// Perform line trace from DetectorOwner to Explorer
	}

	/*
	if(FogOfWar)
	{
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // Ignore the detector itself
		Params.AddIgnoredActor(FogOfWar); 

		FogOfWar->DisableFog();
	}*/
	
	
}

void AFogDetector::InitOverlaps()
{
	DetectorMesh->OnComponentEndOverlap.AddDynamic(this, &AFogDetector::OnDetectorOverlapEnd); 
	DetectorMesh->OnComponentBeginOverlap.AddDynamic(this, &AFogDetector::OnDetectorOverlap);
}


void AFogDetector::OnDetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFogExplorer* Explorer = Cast<AFogExplorer>(OtherActor);
	if(Explorer && !Explorer->IsVisibleInFog && DetectorOwner)
	{
		DetectUnits.Remove(Explorer);
		Explorer->SetActorHiddenInGame(true);
		Explorer->IsVisible = false;
	}
	/*
	AFogOfWar* FogOfWar = Cast<AFogOfWar>(OtherActor);

	if(FogOfWar)
	{
		FogOfWar->EnableFog();
	}
	*/
}

void AFogDetector::CheckForLos()
{

	for (AActor* Actor : DetectUnits)
	{
		FCollisionQueryParams QueryParams(FName(TEXT("SphereTrace")), true, this);
		QueryParams.bTraceComplex = false;
		QueryParams.AddIgnoredActor(this); // Ignore the detector itself
		QueryParams.AddIgnoredActor(DetectorOwner);
		QueryParams.AddIgnoredActor(Actor);
        
		FVector StartLocation = DetectorOwner->GetActorLocation();
		FVector EndLocation = Actor->GetActorLocation();
		float Radius = 100.0f; // Radius of the sphere to check for collisions
        
		TArray<FHitResult> OutHits;
		bool bHit = GetWorld()->SweepMultiByChannel(OutHits, StartLocation, EndLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), QueryParams);

		
		if (!bHit)
		{
			DrawDebugSphere(GetWorld(), EndLocation, Radius, 12, FColor::Green, false, 6.0f, 0, 2.0f);
			Actor->SetActorHiddenInGame(false);
		}
		else
		{
			for (const FHitResult& HitResult : OutHits)
			{
				DrawDebugSphere(GetWorld(), HitResult.Location, Radius, 12, FColor::Red, false, 6.0f, 0, 2.0f);
			}
		}
	}
	
}

// Called when the game starts or when spawned
void AFogDetector::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(TimerHandle_CheckForLos, this, &AFogDetector::CheckForLos, 0.5f, true);
}

void AFogDetector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//GetWorldTimerManager().ClearTimer(TimerHandle_CheckForLos);Super::EndPlay(EndPlayReason);
}

// Called every frame
void AFogDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CheckForLos();
}

