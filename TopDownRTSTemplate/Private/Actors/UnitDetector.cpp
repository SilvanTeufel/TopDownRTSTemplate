// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/UnitDetector.h"
#include "Characters/Unit/SpeakingUnit.h"
#include "GameModes/UnitGeneratorGameModeBase.h"

// Sets default values
AUnitDetector::AUnitDetector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.	PrimaryActorTick.bCanEverTick = true;
	DetectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(DetectorMesh);
	DetectorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryAndPhysics
	DetectorMesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	DetectorMesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	/*
	if (SphereMeshAsset.Succeeded())
	{
		DetectorMesh->SetStaticMesh(SphereMeshAsset.Object);
	}*/
	
	DetectorMesh->OnComponentBeginOverlap.AddDynamic(this, &AUnitDetector::OnDetectorOverlap); 
}

// Called when the game starts or when spawned
void AUnitDetector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnitDetector::OnDetectorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT(" OnDetectorOverlap"));
	if(OtherActor)
	{
			switch (Type)
			{
			case DetectorData::Speech:
				{
					ASpeakingUnit* SpeakingUnit = Cast<ASpeakingUnit>(OtherActor);
					if(SpeakingUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT(" Found Speaker"));
						SpeakingUnit->StartTriggeredSpeechText();
					}
				}
				break;
			case DetectorData::UnitSpawn:
				{
					if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
					{
						AUnitGeneratorGameModeBase* UnitGeneratorGameModeBase = Cast<AUnitGeneratorGameModeBase>(GameMode);
						if (UnitGeneratorGameModeBase)
						{
							// Call a function in the GameMode class
							UnitGeneratorGameModeBase->SpawnEnemysWithID(Id, GetActorLocation()+Offset);
						}
					}
				}
				break;
			case DetectorData::SelectableSpawn:
				{
					
				}
				break;
			case DetectorData::UnitChaseSpawn:
				{
					AUnitBase* UnitBase = Cast<AUnitBase>(OtherActor);

					if(!UnitBase || UnitBase && !UnitBase->IsFriendly) return;
					
					if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
					{
						AUnitGeneratorGameModeBase* UnitGeneratorGameModeBase = Cast<AUnitGeneratorGameModeBase>(GameMode);
						if (UnitGeneratorGameModeBase)
						{
							// Call a function in the GameMode class
							UnitGeneratorGameModeBase->SpawnChaseEnemysWithID(Id, GetActorLocation()+Offset, UnitBase);
						}
					}
	
				
				}
				break;
			default:
				{
		 					
				}
				break;
			}
		
	}
}

void AUnitDetector::OnDetectorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

