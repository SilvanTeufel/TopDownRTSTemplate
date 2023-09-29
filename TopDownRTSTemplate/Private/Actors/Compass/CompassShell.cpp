// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Actors/Compass/CompassShell.h"
#include "Characters/Unit/UnitBase.h"
#include "Actors/Compass/CompassPin.h"
#include "Actors/Compass/CompassPole.h"
#include "Components/SceneComponent.h"

ACompassShell::ACompassShell()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the pin mesh component
    ShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    //SetRootComponent(ShellMesh);

    // Set the collision settings for the mesh
    ShellMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ShellMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    ShellMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

    // Set the mesh to be attached to the root component
    //ShellMesh->SetupAttachment(RootComponent);
    SetRootComponent(ShellMesh);
}

void ACompassShell::BeginPlay()
{
    Super::BeginPlay();
    if (UnitBase)
    {
        // Set the initial rotation of the compass shell to match the forward vector of the UnitBase actor
        SetActorRotation(UnitBase->GetActorForwardVector().Rotation());
    }


}

void ACompassShell::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (UnitBase)
    {
      SetActorRotation(UnitBase->GetActorForwardVector().Rotation());
    }

}


void ACompassShell::Init()
{

}

void ACompassShell::SetUnitBase(AUnitBase* NewUnitBase)
{
    UnitBase = NewUnitBase;
}
