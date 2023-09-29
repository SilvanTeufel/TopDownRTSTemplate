// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Characters/Unit/HoldPositionUnit.h"



void AHoldPositionUnit::SpawnDetector()
{
	
	if (!DetectorClass)
	{
		return;
	}
	FVector SpawnLocation = GetActorLocation()+DetectorLocationOffset;
	FRotator SpawnRotation = GetActorRotation()+DetectorRotation;
	FTransform Transformer;
	
	Transformer.SetLocation(SpawnLocation);
	Transformer.SetRotation(SpawnRotation.Quaternion());
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AHoldPositionDetector* NewDetector = GetWorld()->SpawnActorDeferred<AHoldPositionDetector>(DetectorClass, Transformer);


	
	
	if(NewDetector)
	{
		

		NewDetector->SetActorLocation(SpawnLocation);
		NewDetector->FinishSpawning(Transformer);
		NewDetector->InitDetector(this);

		USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());

		NewDetector->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("DetectorSocket")); 
		NewDetector->SetActorRotation(SpawnRotation);

		Detector = NewDetector; 
	}

}

void AHoldPositionUnit::DetectorUnLock()
{

	
		if (Detector) {
			FVector Location = Detector->GetActorLocation();
			FRotator Rotation = Detector->GetActorRotation();
			Detector->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			Detector->SetActorLocation(Location);
			Detector->SetActorRotation(Rotation);
		}

}