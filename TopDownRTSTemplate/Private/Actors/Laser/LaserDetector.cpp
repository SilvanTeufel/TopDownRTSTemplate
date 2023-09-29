// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Laser/LaserDetector.h"

#include "Actors/Abilitys/GravityActor.h"
#include "Actors/Laser/LaserBeam.h"
#include "Actors/Laser/TriggeredMovementActor.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"
// Sets default values
ALaserDetector::ALaserDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DetectorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Prism Mesh");
	
	// Set the collision settings for the mesh
	DetectorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DetectorMesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	DetectorMesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren
	
	//RootComponent = PrismMesh;
	//DetectorMesh->SetupAttachment(RootComponent);
	//DetectorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // QueryAndPhysics
	DetectorMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Plane"));
	if (SphereMeshAsset.Succeeded())
	{
		DetectorMesh->SetStaticMesh(SphereMeshAsset.Object);
		DetectorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));

		static ConstructorHelpers::FObjectFinder<UMaterial>MaterialBlue(_T("Material'/TopDownRTSTemplate/TopDownRTSTemplate/Materials/M_Ring_Aura.M_Ring_Aura'"));
		static ConstructorHelpers::FObjectFinder<UMaterial>MaterialAction(_T("Material'/TopDownRTSTemplate/TopDownRTSTemplate/Materials/M_Ring_Aura_Red.M_Ring_Aura_Red'"));

		if (MaterialBlue.Object != NULL)
		{
			BlueMaterial = (UMaterial*)MaterialBlue.Object;
			DetectorMesh->SetMaterial(0, BlueMaterial);
		}

		if (MaterialAction.Object != NULL)
		{
			RedMaterial = (UMaterial*)MaterialAction.Object;
			DetectorMesh->SetMaterial(0, RedMaterial);
		}

		DetectorMesh->bHiddenInGame = true;
	}
}

// Called when the game starts or when spawned
void ALaserDetector::BeginPlay()
{
	Super::BeginPlay();

	DetectorMesh->OnComponentEndOverlap.AddDynamic(this, &ALaserDetector::OnDetectorOverlapEnd);
	DetectorMesh->OnComponentBeginOverlap.AddDynamic(this, &ALaserDetector::OnDetectorOverlap);
	ChangeMaterialColour(FVector4d(5.f, 10.f, 100.f, 0.5f));
}

void ALaserDetector::OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnPrismOverlap!"));
	ALaserBeam* Beam = Cast<ALaserBeam>(OtherActor);
	AGravityActor* GravityActor = Cast<AGravityActor>(OtherActor);
	
	if(Beam || GravityActor)
	{
		ChangeMaterialColour(FVector4d(150.f, 10.f, 10.f, 0.5f));
		Enabled = true;
		ControlTimer = 0.f;
	}
}

void ALaserDetector::OnDetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	StartTick = true;
}
// Called every frame
void ALaserDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(StartTick)
	{
		ControlTimer += DeltaTime;
	
		if(ControlTimer > DisableDetectorTime)
		{
			ControlTimer = 0.f;
			ChangeMaterialColour(FVector4d(5.f, 10.f, 100.f, 0.5f));
			Enabled = false;
			StartTick = false;
		}
	}

}

void ALaserDetector::ChangeMaterialColour(FVector4d Colour)
{
	DynMaterial = UMaterialInstanceDynamic::Create(BlueMaterial, this);
	DynMaterial->SetVectorParameterValue(TEXT("RingColor"), Colour); //FVector4d(100.f, 10.f, 5.f, 0.5f)
	DetectorMesh->SetMaterial(0, DynMaterial);
}

