// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/FogOfWar.h"
#include "TimerManager.h"


// Sets default values
AFogOfWar::AFogOfWar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FogMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(FogMesh);
	FogMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryAndPhysics
	FogMesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	FogMesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren
}

// Called when the game starts or when spawned
void AFogOfWar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFogOfWar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetFogVisibility(Visible);
}

void AFogOfWar::InitOverlaps()
{
	FogMesh->OnComponentEndOverlap.AddDynamic(this, &AFogOfWar::OnDetectorOverlapEnd); 
	FogMesh->OnComponentBeginOverlap.AddDynamic(this, &AFogOfWar::OnDetectorOverlap);
}

void AFogOfWar::SetFogVisibility(bool bVisible)
{
	UMaterialInstanceDynamic* DynamicMaterial = FogMesh->CreateDynamicMaterialInstance(0);
	
	if (DynamicMaterial)
	{
		FName ParameterName("Visibility");
		float CurrentValue;
		DynamicMaterial->GetScalarParameterValue(TEXT("Visibility"), CurrentValue);
		
			float TargetValue = bVisible ? 1.0f : 0.0f;

			// Adjust the interpolation speed according to your preference
			float InterpolationSpeed = 2.0f;

			// Interpolate between the current value and the target value manually
			float DeltaValue = TargetValue - CurrentValue;
			float InterpolatedValue = CurrentValue + (DeltaValue * InterpolationSpeed * GetWorld()->GetDeltaSeconds());

			// Clamp the interpolated value to ensure it stays within the valid range
			InterpolatedValue = FMath::Clamp(InterpolatedValue, 0.0f, 1.f);

			// Set the interpolated value as the new scalar parameter value
			DynamicMaterial->SetScalarParameterValue(TEXT("Visibility"), InterpolatedValue);
		
	}
}

void AFogOfWar::OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("AFogOfWar::OnDetectorOverlap!"));
	AFogExplorer* FogExplorer = Cast<AFogExplorer>(OtherActor);

	
	if(FogExplorer && !FogExplorer->IsVisibleInFog && Visible)
	{
		FogExplorer->SetActorHiddenInGame(true);
		ExplorersGetHidden.Add(FogExplorer);
	}else if(FogExplorer && FogExplorer->IsVisibleInFog && Visible)
	{
		DisableFog();
	}
	
}

void AFogOfWar::OnDetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AFogExplorer* FogExplorer = Cast<AFogExplorer>(OtherActor);
	
	if(FogExplorer && !FogExplorer->IsVisibleInFog)
	{
		ExplorersGetHidden.Remove(FogExplorer);
	}
}

void AFogOfWar::DisableFog()
{
	for(int i = 0; i < ExplorersGetHidden.Num(); i++)
	{
		if(ExplorersGetHidden[i])
		{
			ExplorersGetHidden[i]->SetActorHiddenInGame(false);
		}
	}
	Visible = false;
}

void AFogOfWar::EnableFog()
{
	for(int i = 0; i < ExplorersGetHidden.Num(); i++)
	{
		if(ExplorersGetHidden[i])
		{
			ExplorersGetHidden[i]->SetActorHiddenInGame(true);
		}
	}
	Visible = true;
}
