// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Abilitys/HealingActor.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AHealingActor::AHealingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryAndPhysics
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetMaterial(0, Material);
	Mesh->SetSimulatePhysics(false);
	
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Is PHealing Capsule"));
	TriggerCapsule->InitCapsuleSize(100.f, 100.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AHealingActor::OnOverlapBegin);

}

void AHealingActor::Init(AUnitBase* Target, AUnitBase* Healer)
{
	HealingUnit = Healer;
	Target->SetHealth(Target->GetHealth() + MainHeal);
}

// Called when the game starts or when spawned
void AHealingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ControlTimer = ControlTimer + DeltaTime;
	if(ControlTimer >= HealIntervalTime){
		ControlTimer = 0.f;
		ControlInterval++;
		if(ControlInterval > MaxIntervals) Destroy(true);
		
		for (int32 i = 0; i < Actors.Num(); i++)
		{
			if(Actors[i])
			Actors[i]->SetHealth(Actors[i]->GetHealth() + IntervalHeal);
		}
	}
}


void AHealingActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AUnitBase* UnitToHeal = Cast<AUnitBase>(OtherActor);

		if(UnitToHeal && HealingUnit && HealingUnit->IsFriendly == UnitToHeal->IsFriendly)
			Actors.Push(UnitToHeal);
			
	}
}
