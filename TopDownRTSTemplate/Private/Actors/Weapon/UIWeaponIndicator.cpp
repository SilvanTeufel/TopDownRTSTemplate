// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Weapon/UIWeaponIndicator.h"

// Sets default values
AUIWeaponIndicator::AUIWeaponIndicator()
{
 	
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    if (WeaponMesh) {
        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // QueryAndPhysics
        //WeaponMesh->SetupAttachment(RootComponent);
        SetRootComponent(WeaponMesh);
        if(WeaponMesh)
        {
            WeaponMesh->SetRelativeLocation(WeaponIndicatorPosition);
            FQuat QuadRotation = FQuat(WeaponIndicatorRotation);
            WeaponMesh->SetRelativeRotation(QuadRotation); // FVector(90.f, 0.f, 0.f)
            WeaponMesh->SetCastShadow(false);
        }
    }
}

// Called when the game starts or when spawned
void AUIWeaponIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIWeaponIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUIWeaponIndicator::ChangeWeaponIndicator(USkeletalMesh* NewWeaponMesh)
{
    if (WeaponMesh)
    {
        WeaponMesh->SetSkeletalMesh(NewWeaponMesh);
        WeaponMesh->SetCastShadow(false);
        WeaponMesh->SetVisibility(true);
    }
    else
    {
        // Hide the mesh when WeaponMesh is not valid
        if (WeaponMesh->IsVisible())
        {
            WeaponMesh->SetVisibility(false);
        }
    }
}

