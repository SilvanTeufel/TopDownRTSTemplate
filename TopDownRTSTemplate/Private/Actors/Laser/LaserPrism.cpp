// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Laser/LaserPrism.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ALaserPrism::ALaserPrism()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrismMesh = CreateDefaultSubobject<UStaticMeshComponent>("Prism Mesh");
	
	// Set the collision settings for the mesh
	PrismMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PrismMesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	PrismMesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren
	
	// Moblility
	PrismMesh->SetMobility(EComponentMobility::Movable);
	//RootComponent = PrismMesh;
	PrismMesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ALaserPrism::BeginPlay()
{
	Super::BeginPlay();
	
	
	//PrismMesh->OnComponentBeginOverlap.AddDynamic(this, &ALaserPrism::OnPrismOverlap);
	PrismMesh->OnComponentEndOverlap.AddDynamic(this, &ALaserPrism::OnPrismOverlapEnd);

	
}

void ALaserPrism::OnPrismOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ALaserPrism::OnPrismOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	ALaserBeam* Beam = Cast<ALaserBeam>(OtherActor);

	if(Beam && OutBeam != Beam && !Beam->IsIndicator)
	{
		InBeam = Beam;
		SpawnNewLaser = true;
	}
}

void ALaserPrism::SpawnWithTimer(float DeltaTime)
{
	SpawnControlTimer = SpawnControlTimer+DeltaTime;
	if(InBeam && SpawnNewLaser && SpawnControlTimer >= SpawnCoolDown ) //  && SpawnControlTimer >= SpawnCoolDown
	{
		OutBeam = SpawnLaser(GetActorLocation(), InBeam->GetActorRotation());
		SpawnControlTimer = 0.f;
	}
}
// Called every frame
void ALaserPrism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpawnWithTimer(DeltaTime);
}

ALaserBeam* ALaserPrism::SpawnLaser(FVector Location, FRotator Rot)
{
	
	
	if (!LaserBeamClass)
	{
		return nullptr;
	}
	
	FVector SpawnLocation = Location;
	FRotator SpawnRotation = Rot+Rotation;
	FTransform Transformer;
	
	Transformer.SetLocation(SpawnLocation);
	Transformer.SetRotation(SpawnRotation.Quaternion());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	// Set the collision channel of the spawned actor
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	
	ALaserBeam* NewLaserBeam = GetWorld()->SpawnActorDeferred<ALaserBeam>(LaserBeamClass, Transformer);

	if (NewLaserBeam)
	{
		NewLaserBeam->AttachToComponent(PrismMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LaserSocket"));
		
		// Set the scale of the laser beam actor
		//float LaserScale = 2.f; // Change this value as needed
		NewLaserBeam->SetActorScale3D(NewLaserBeam->Scaler);
		NewLaserBeam->SetActorRotation(SpawnRotation);
		NewLaserBeam->SetActorLocation(SpawnLocation);
	
		NewLaserBeam->FinishSpawning(Transformer);
	
		FVector RotatedVector = RotateVectorAroundPoint(InBeam->GetActorLocation(), GetActorLocation(), Rotation);

		//FVector Direction = RotatedVector - GetActorLocation();
		//Direction.Normalize();
		// Rotate the forward vector using the rotator
		NewLaserBeam->DirectionVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), RotatedVector);
		NewLaserBeam->LaserMesh->OnComponentBeginOverlap.AddDynamic(NewLaserBeam, &ALaserBeam::OnLaserOverlap);
		NewLaserBeam->LaserMesh->OnComponentEndOverlap.AddDynamic(NewLaserBeam, &ALaserBeam::OnLaserEndOverlap);
		
		return NewLaserBeam;
	}
	
	return nullptr;

}