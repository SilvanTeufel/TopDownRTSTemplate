// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Characters/Unit/LaserUnit.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Math/Vector.h"

// Called when the game starts or when spawned
void ALaserUnit::BeginPlay()
{
	Super::BeginPlay();
	
}
void ALaserUnit::SetControlTimer(float Time)
{
	ControlTimer = Time;
}
// Called every frame
void ALaserUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RestoreEnergy(DeltaTime);
	switch (UnitState){
		case UnitData::LaserLoading:
		{
				if (ActiveLaserIndicator) // && !LaserBeams.Num()
				{
					ControlTimer += DeltaTime;
					if(ControlTimer > LaserLoadTime)
					{
						if(Energy > EnergyCost)
						{
							
							ActiveLaserIndicator->Destroy();
							ActiveLaserIndicator = nullptr;
							int z = 1;

							SpawnLaser(GetActorLocation()+GetActorForwardVector()*600.f+SpawnOffset,FRotator(0.f,0.f,0.f));
							
							for(int i = 0; i < LaserCount; i++)
							{
							
								FVector RotatedVector;
								FVector ForwardVector = GetActorForwardVector();

								if(i % 2 == 0)
								{
									z++;
									RotatedVector = FRotator(0.f,90.f,0.f).RotateVector(ForwardVector);
								}
								else
									RotatedVector = FRotator(0.f,-90.f,0.f).RotateVector(ForwardVector);
					
								
								SpawnLaser(GetActorLocation()+GetActorForwardVector()*600.f+SpawnOffset+z*20.f*RotatedVector,FRotator(0.f,0.f,0.f));
							}
							//SpawnLaser(GetActorLocation()+GetActorForwardVector()*400.f+SpawnOffset,FRotator(0.f,0.f,0.f));
							ControlTimer = 0.f;
							Energy = Energy - EnergyCost;
							SetUnitState(UnitData::LaserFire);
						}else
						{
							SetUnitState(UnitData::NoMana);
						}
					}
				}
		}
		break;
		case UnitData::LaserFire:
		{
				ControlTimer += DeltaTime;
				if(ControlTimer > LaserFireTime)
				{
					SetUnitState(UnitData::Idle);
					ControlTimer = 0.f;
				}
		}
		break;
		default:
		{

		}
		break;
	}	
}

void ALaserUnit::RestoreEnergy(float DeltaTime)
{

	EnergyTimer += DeltaTime;

	if(EnergyTimer > EnergyRestoreTime)
		if((Energy + EnergyToRestore) >= MaxEnergy)
		{
			Energy = MaxEnergy;
			EnergyTimer = 0.f;
		}else
		{
			Energy += EnergyToRestore;
			EnergyTimer = 0.f;
		}
	
}

void ALaserUnit::AddEnergy(float EnergyToAdd)
{
	if((Energy + EnergyToAdd) >= MaxEnergy)
	{
		Energy = MaxEnergy;
	}else
	{
		Energy += EnergyToAdd;
	}
}

void ALaserUnit::SpawnLaserIndicator(FVector Location, FRotator RotationOffset)
{
	if (!LaserIndicatorClass)
	{
		return;
	}

	if (ActiveLaserIndicator)
	{
		ActiveLaserIndicator->Destroy();
		ActiveLaserIndicator = nullptr;
	}

	FVector SpawnLocation = Location;
	FRotator SpawnRotation = RotationOffset + GetActorRotation() + FRotator(90.f, 0.f, 0.f);
	FTransform Transformer;
	
	Transformer.SetLocation(SpawnLocation);
	Transformer.SetRotation(SpawnRotation.Quaternion());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ALaserBeam* NewLaserBeam = GetWorld()->SpawnActorDeferred<ALaserBeam>(LaserIndicatorClass, Transformer);
	USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
	NewLaserBeam->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LaserSocket"));
	
	NewLaserBeam->SetActorScale3D(FVector(0.01f, 0.01f, 15.f));
	NewLaserBeam->SetActorRotation(SpawnRotation);
	NewLaserBeam->SetActorLocation(SpawnLocation);

	NewLaserBeam->FinishSpawning(Transformer);
	NewLaserBeam->IsIndicator = true;
	ActiveLaserIndicator = NewLaserBeam;

	
}

void ALaserUnit::DestroyLaserIndicator()
{
	
	if(ActiveLaserIndicator)
	{
		ActiveLaserIndicator->Destroy();
		ActiveLaserIndicator = nullptr;
		SetControlTimer(0.f);
	}
	
}

ALaserBeam* ALaserUnit::SpawnLaser(FVector Location, FRotator RotationOffset)
{
	if (!LaserBeamClass)
	{
		return nullptr;
	}

	FVector SpawnLocation = Location;
	FRotator SpawnRotation = RotationOffset + GetActorRotation() + FRotator(90.f, 0.f, 0.f);
	FTransform Transformer;
	
	Transformer.SetLocation(SpawnLocation);
	Transformer.SetRotation(SpawnRotation.Quaternion());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ALaserBeam* NewLaserBeam = GetWorld()->SpawnActorDeferred<ALaserBeam>(LaserBeamClass, Transformer);

	if(NewLaserBeam)
	{
		NewLaserBeam->SetActorScale3D(NewLaserBeam->Scaler);
		NewLaserBeam->SetActorRotation(SpawnRotation);
		NewLaserBeam->SetActorLocation(SpawnLocation);
		FVector DVector = GetActorForwardVector();
		NewLaserBeam->DirectionVector = DVector;
		NewLaserBeam->LaserMesh->OnComponentBeginOverlap.AddDynamic(NewLaserBeam, &ALaserBeam::OnLaserOverlap);
		NewLaserBeam->LaserMesh->OnComponentEndOverlap.AddDynamic(NewLaserBeam, &ALaserBeam::OnLaserEndOverlap);

		NewLaserBeam->FinishSpawning(Transformer);
		LaserBeams.Push(NewLaserBeam);
		
		return NewLaserBeam;
	}
	return nullptr;
}

// Called to bind functionality to input
void ALaserUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

