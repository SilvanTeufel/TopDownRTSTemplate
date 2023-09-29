// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Abilitys/Shield.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
AShield::AShield()
{
    PrimaryActorTick.bCanEverTick = true;
    SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
    SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // QueryAndPhysics
    //SphereMesh->SetupAttachment(RootComponent);
    SetRootComponent(SphereMesh);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
    if (SphereMeshAsset.Succeeded())
    {
        SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
        SphereMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
        SphereMesh->SetWorldScale3D(FVector(6.f));

        static ConstructorHelpers::FObjectFinder<UMaterial>MaterialA(_T("Material'/TopDownRTSTemplate/TopDownRTSTemplate/Materials/M_Shield.M_Shield'"));

        if (MaterialA.Object != NULL)
        {
            BlueMaterial = (UMaterial*)MaterialA.Object;
            SphereMesh->SetMaterial(0, BlueMaterial);
        }


        static ConstructorHelpers::FObjectFinder<UMaterial>MaterialB(_T("Material'/TopDownRTSTemplate/TopDownRTSTemplate/Materials/M_Transparent.M_Transparent'"));

        if (MaterialB.Object != NULL)
        {
            HiddenMaterial = (UMaterial*)MaterialB.Object;
            
        }
    }

    IsActive = false;
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    RefreshCounterTick(DeltaTime);
}

void AShield::Hide()
{
    SphereMesh->SetMaterial(0, HiddenMaterial);
}

void AShield::Show()
{
    SphereMesh->SetMaterial(0, BlueMaterial);
}

void AShield::RefreshCounterTick(float DeltaTime)
{
        if (HitCounterRefreshTime >= HitCounterMaxRefreshTime && !IsActive) {
            if(HitCounter < MaxHitCounter)
                HitCounter++;
            HitCounterRefreshTime = 0.f;
        }
        else {
          HitCounterRefreshTime += DeltaTime;
        }
}
