// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Weapon/WeaponBase.h"

#include "AI/NavigationSystemBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/MovementComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	MeshComp->SetCollisionProfileName(NormalCollisionProfile.Name, true);
	MeshComp->SetSimulatePhysics(false);
	
	RootComponent = MeshComp;
	
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Is Weapon Capsule"));
	TriggerCapsule->InitCapsuleSize(30.f, 30.0f);;
	TriggerCapsule->SetCollisionProfileName(NormalCollisionProfile.Name);
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::WeaponOverlap);
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		switch (WeaponState)
		{
		case WeaponStates::Flying:
			{
				if(UnitWhichThrowed)
					WeaponIsFlying(UnitWhichThrowed);
			}
			break;
		case WeaponStates::Returning:
			{
				if(UnitWhichThrowed)
					WeaponIsReturning(UnitWhichThrowed);
			}
			break;
		case WeaponStates::Normal:
			{
				const FVector currentVelocity = MeshComp->GetPhysicsLinearVelocity();
				const FVector clampedVelocity = currentVelocity.GetClampedToMaxSize(MaxWeaponNormalVelocity);
				MeshComp->SetPhysicsLinearVelocity(clampedVelocity);
			}
			break;
		}
	
}

void AWeaponBase::WeaponIsFlying(AExtendedUnitBase* ExtendedUnitBase)
{
	if(ExtendedUnitBase && ExtendedUnitBase->UnitToThrowWeapon && ExtendedUnitBase->Weapon)
	{
		if(!ExtendedUnitBase->WeaponUnlocked) ExtendedUnitBase->WeaponUnLock(); ExtendedUnitBase->WeaponUnlocked = true;

		FVector CurrentActorLocation = ExtendedUnitBase->Weapon->GetActorLocation();
		
		FVector DirectionVector = UKismetMathLibrary::GetDirectionUnitVector(CurrentActorLocation, ExtendedUnitBase->UnitToThrowWeapon->GetActorLocation());
		ExtendedUnitBase->Weapon->SetActorLocation(CurrentActorLocation+DirectionVector*ExtendedUnitBase->ThrowScaler);
		
		FRotator CurrentRotation = ExtendedUnitBase->Weapon->GetActorRotation();
		ExtendedUnitBase->Weapon->SetActorRotation(CurrentRotation + FRotator( 0.f, 0.f, 15.f));
	}
}

void AWeaponBase::WeaponIsReturning(AExtendedUnitBase* ExtendedUnitBase)
{
	const FVector CurrentActorLocation = ExtendedUnitBase->Weapon->GetActorLocation();
	const FVector ExtendedUnitBaseLocation = ExtendedUnitBase->GetActorLocation();
	const FVector DirectionVector = UKismetMathLibrary::GetDirectionUnitVector(CurrentActorLocation, ExtendedUnitBaseLocation);
	ExtendedUnitBase->Weapon->SetActorLocation(CurrentActorLocation+DirectionVector*ExtendedUnitBase->GravityScaler);

	const FRotator CurrentRotation = ExtendedUnitBase->Weapon->GetActorRotation();
	ExtendedUnitBase->Weapon->SetActorRotation(CurrentRotation + FRotator( 0.f, 0.f, 15.f));
	const FVector VectorDiff = ExtendedUnitBaseLocation-CurrentActorLocation;
	const float DistanceToWeapon = sqrt(VectorDiff.X*VectorDiff.X + VectorDiff.Y*VectorDiff.Y + VectorDiff.Z*VectorDiff.Z);
	if(DistanceToWeapon < 50.f )
	{
		ExtendedUnitBase->Weapon->SetActorRotation(CurrentRotation + FRotator( 0.f, 0.f, 0.f));
		ExtendedUnitBase->WeaponLock();
		ExtendedUnitBase->WeaponUnlocked = false;
		ExtendedUnitBase->UnitDidThrowWeapon = false;
		SetWeaponState(WeaponStates::Normal);
	}
}


void AWeaponBase::WeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		
		AUnitBase* UnitToHit = Cast<AUnitBase>(OtherActor);
		if(UnitToHit && !UnitToHit->IsFriendly && UnitWhichThrowed)
		{
			MeshComp->SetCollisionProfileName(ThrowCollisionProfile.Name, true);
			MeshComp->SetSimulatePhysics(true);
			UnitToHit->StunOffset = UnitWhichThrowed->TalentWeaponStunOffset;
			UnitToHit->SetHealth(UnitToHit->GetHealth() - GetWeaponThrowDamage(UnitWhichThrowed->Weapons[UnitWhichThrowed->ActualWeaponId]));
			UnitToHit->SetUnitState( UnitData::Stunned );
			UnitToHit->UnitControlTimer = 0.f;
			SetWeaponState(WeaponStates::Normal);
			
		}
	}
}



// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<FString> WeaponNames = GetWeaponNames();

	if (WeaponDataTable)
	{
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponNames[1]), WeaponNames[1], true); // AR-15 // AS-VAL
		if (WeaponData) {
			MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
		}
	}
}

void AWeaponBase::SetWeaponFlying(AExtendedUnitBase* Target)
{
	SetWeaponState(WeaponStates::Flying);
	UnitWhichThrowed = Target;
	
}

void AWeaponBase::SetWeaponReturning(AExtendedUnitBase* Target)
{
	MeshComp->SetCollisionProfileName(NormalCollisionProfile.Name, true);
	MeshComp->SetSimulatePhysics(false);
	SetWeaponState(WeaponStates::Returning);
	UnitWhichThrowed = Target;
}

void AWeaponBase::ChangeWeapon(FString WeaponName) {
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		if (WeaponData) {
			MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
		}
	}
}

float AWeaponBase::GetWeaponRange(FString WeaponName)
{

	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Range;
	}
	return 0.f;
}

int AWeaponBase::GetWeaponMagSize(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->MagSize;
	}
	return 0;
}

float AWeaponBase::GetWeaponDamage(FString WeaponName)
{

	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Damage;
	}
	return 0.f;
}

float AWeaponBase::GetWeaponThrowDamage(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ThrowDamage;
	}
	return 10.f;
}

float AWeaponBase::GetWeaponThrowRange(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ThrowRange;
	}
	return 100.f;
}

float AWeaponBase::GetWeaponTime(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Time;
	}
	return 0.f;
}

FString AWeaponBase::GetWeaponType(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Type;
	}
	return "Unknown";
}

USkeletalMesh* AWeaponBase::GetWeaponMesh(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->WeaponMesh;
	}
	return nullptr;
}

TArray<FString> AWeaponBase::GetWeaponNames()
{
	if (WeaponDataTable)
	{
		TArray<FName> Names = WeaponDataTable->GetRowNames();
		TArray<FString> SNames;
		for (int Index = 0; Index != Names.Num(); ++Index)
		{
			SNames.Emplace(Names[Index].ToString());
			
		}
		return TArray<FString>(SNames);
	}
	else {
		return TArray<FString>();
	}
}

UStaticMesh* AWeaponBase::GetProjectileMesh(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileMesh;
	}
	return nullptr;
}

UMaterialInterface* AWeaponBase::GetProjectileMaterial(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileMaterial;
	}
	return nullptr;
}

FVector AWeaponBase::GetProjectileScale(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileScale;
	}
	return FVector(1.f,1.f,1.f);
}

float AWeaponBase::GetReloadTime(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ReloadTime;
	}
	return 1.f;
}

float AWeaponBase::GetProjectileSpeed(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileSpeed;
	}
	return 1.f;
}

float AWeaponBase::GetPauseDuration(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->PauseDuration;
	}
	return 1.f;
}

int AWeaponBase::GetMaxPiercedTargets(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->MaxPiercedTargets;
	}
	return 1;
}

void AWeaponBase::PlayFireAnimation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Starting PlayFireAnimation Animation!"));

	if (WeaponData && WeaponData->FireAnimation) {
		MeshComp->PlayAnimation(WeaponData->FireAnimation, false);
	}
}
