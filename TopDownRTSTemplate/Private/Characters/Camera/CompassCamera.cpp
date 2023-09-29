// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Characters/Camera/CompassCamera.h"

// Called when the game starts or when spawned
void ACompassCamera::BeginPlay()
{
	Super::BeginPlay();
	CreateCompassShell();
	SpawnCompassPins();
}

// Called every frame
void ACompassCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PositionCompassShell();
	PositionCompassPins();
}

// Called to bind functionality to input
void ACompassCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ACompassCamera::CreateCompassShell()
{
	if (CompassShellClass)
	{
		// Spawn the compass shell
		CompassShell = GetWorld()->SpawnActor<ACompassShell>(CompassShellClass, GetActorLocation()+PositionOffset, GetActorRotation());
		if (CompassShell)
		{
			// Set the parent of the compass shell to this camera base
			CompassShell->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			CompassShell->SetActorScale3D(FVector(ShellScaler));
			CompassShell->Init();
		}
	}
}

void ACompassCamera::PositionCompassShell()
{
	if (CompassShell)
	{
		CompassShell->SetActorRelativeLocation(FVector(PositionOffset.X,PositionOffset.Y, PositionOffset.Z));
	}
}

void ACompassCamera::SpawnCompassPins()
{
	// Iterate through the array of compass pins
	if(CompassShell)
		for (int32 i = 0; i < CompassShell->CompassPinClasses.Num(); i++)
		{
			// Spawn an instance of the compass pin at the character's location
			if(CompassShell->CompassPinClasses[i])
			{
				ACompassPin* Pin = GetWorld()->SpawnActor<ACompassPin>(CompassShell->CompassPinClasses[i], GetActorLocation()+FVector(0.f,0.f, 1.f*i)+PositionOffset, FRotator::ZeroRotator);
        
				// Attach the pin to the character's root component
				Pin->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
				Pin->SetActorScale3D(FVector(PinScaler));
				
				ACompassPole* Pole = GetWorld()->SpawnActor<ACompassPole>(Pin->CompassPoleClass, Pin->CompassPoleLocation, FRotator::ZeroRotator);
			
				if(Pole)
				{
					Pin->SetCompassPole(Pole);
					CompassPins.Push(Pin);
				}
			}
		}
}

void ACompassCamera::PositionCompassPins()
{
	for (int32 i = 0; i < CompassPins.Num(); i++)
	{
		CompassPins[i]->SetActorRelativeLocation(FVector(PositionOffset.X,PositionOffset.Y, PositionOffset.Z+1.f*i));
	}
}

