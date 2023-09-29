// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Selectable.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/TalentedUnit.h"
#include "Characters/Unit/ExtendedUnitBase.h"

// Sets default values
ASelectable::ASelectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Is Selectable Capsule"));
	TriggerCapsule->InitCapsuleSize(30.f, 30.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASelectable::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASelectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASelectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(OtherActor);
		
		 if(ExtendedUnitBase && ExtendedUnitBase->IsFriendly)
		{
		 	if(ExtendedUnitBase)
		 		switch (Type)
		 		{
		 	
					case SelectableData::Health:
		 				{
							ExtendedUnitBase->SetHealth(ExtendedUnitBase->GetHealth() + Amount);
		 				}
		 			break;
		 			case SelectableData::Mana:
		 				{
		 					ExtendedUnitBase->SetMana(ExtendedUnitBase->GetMana() + Amount);
		 				}
		 					break;
		 			case SelectableData::Ammo:
		 				{
		 					ExtendedUnitBase->SetAmmo(ExtendedUnitBase->GetAmmo() + Amount);
		 				}
		 			break;
		 			case SelectableData::Magazine:
		 				{
		 					ExtendedUnitBase->AddMagCount(WeaponId, Amount);
		 				}
		 			break;
		 			case SelectableData::WeaponTalentPoint:
		 			{
		 					ATalentedUnit* TalentedUnit = Cast<ATalentedUnit>(ExtendedUnitBase);
		 					if(TalentedUnit)
		 				TalentedUnit->AddTalentPoint(Amount);
		 			}
		 			break;
		 			default:
		 				{
		 					
		 				}
		 			break;
		 		}

		 	if(Sound)
		 	UGameplayStatics::PlaySoundAtLocation(ExtendedUnitBase, Sound, ExtendedUnitBase->GetActorLocation(), 1.f);
		 	
			Destroy(true, false);
		}
			
	}
}

