// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Characters/Unit/ExtendedUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/SelectedIcon.h"
#include "Components/SceneComponent.h"
#include "Actors/Weapon/WeaponBase.h"

// AExtendedUnitBase constructor
AExtendedUnitBase::AExtendedUnitBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// AExtendedUnitBase initialization code
	
}

void AExtendedUnitBase::BeginPlay()
{
	Super::BeginPlay();

	WeaponSpawn();
	SpawnShield();
	


	UseProjectile = true;
	ToggleUnitDetection = false;
	 //MagSize = MaxMagSize;
}

void AExtendedUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RestoreMana(DeltaTime);

}

// Weaponbase related //////////////////////////////////////////////////
void AExtendedUnitBase::WeaponSpawn()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass) {

		FTransform WeaponTransform;
		WeaponTransform.SetLocation(FVector::ZeroVector);
		WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, WeaponTransform, SpawnParams);

		if (Weapon) {
			Weapons = Weapon->GetWeaponNames();
			USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
			Weapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
	

			// GETTING PROJECTILE INFORMATION FROM THE DATATABLE //
			ProjectileMaterial = Weapon->GetProjectileMaterial(Weapons[ActualWeaponId]);
			ProjectileMesh = Weapon->GetProjectileMesh(Weapons[ActualWeaponId]);
			ProjectileScale =  Weapon->GetProjectileScale(Weapons[ActualWeaponId]);
			AttackDamage = Weapon->GetWeaponDamage(Weapons[ActualWeaponId]);
			AttackDuration = Weapon->GetWeaponTime(Weapons[ActualWeaponId]);
			Range = Weapon->GetWeaponRange(Weapons[ActualWeaponId]);
			ReloadTime = Weapon->GetReloadTime(Weapons[ActualWeaponId]);
			ProjectileSpeed =  Weapon->GetProjectileSpeed(Weapons[ActualWeaponId]);
			PauseDuration = Weapon->GetPauseDuration(Weapons[ActualWeaponId]);
			ThrowRange = Weapon->GetWeaponThrowRange(Weapons[ActualWeaponId]);
			MaxPiercedTargets = Weapon->GetMaxPiercedTargets(Weapons[ActualWeaponId]);
		}
		
		for (int i = 0; i < Weapons.Num(); i++) {
			MaxMagSize.Emplace(Weapon->GetWeaponMagSize(Weapons[i]));
			MagCount.Emplace(0);
			MagSize.Emplace(Weapon->GetWeaponMagSize(Weapons[i]));
		}
	}
	
}

void AExtendedUnitBase::WeaponLock()
{

	if (WeaponClass) {
		if (Weapon) {
			USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
			Weapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
		}
	}
}

void AExtendedUnitBase::WeaponUnLock()
{

	if (WeaponClass) {
		if (Weapon) {
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			Weapon->SetActorLocation(GetActorLocation());
		}
	}
}

void AExtendedUnitBase::ChangeWeapon(FString WeaponName) {
	if (WeaponClass) {
		if (Weapon) {
			// To Handsocket
			Weapon->ChangeWeapon(WeaponName);

			// GETTING PROJECTILE INFORMATION FROM THE DATATABLE //
			ProjectileMaterial = Weapon->GetProjectileMaterial(Weapons[ActualWeaponId]);
			ProjectileMesh = Weapon->GetProjectileMesh(Weapons[ActualWeaponId]);
			ProjectileScale =  Weapon->GetProjectileScale(Weapons[ActualWeaponId]);
			AttackDamage = Weapon->GetWeaponDamage(Weapons[ActualWeaponId]);
			AttackDuration = Weapon->GetWeaponTime(Weapons[ActualWeaponId]);
			Range = Weapon->GetWeaponRange(Weapons[ActualWeaponId]);
			ReloadTime = Weapon->GetReloadTime(Weapons[ActualWeaponId]);
			ProjectileSpeed =  Weapon->GetProjectileSpeed(Weapons[ActualWeaponId]);
			PauseDuration = Weapon->GetPauseDuration(Weapons[ActualWeaponId]);
			ThrowRange = Weapon->GetWeaponThrowRange(Weapons[ActualWeaponId]);
			MaxPiercedTargets = Weapon->GetMaxPiercedTargets(Weapons[ActualWeaponId]);
			MaxMagSize[ActualWeaponId] = Weapon->GetWeaponMagSize(Weapons[ActualWeaponId]);
		}
	}
}

void AExtendedUnitBase::SetWeaponFlying() {
	if (WeaponClass) {
		if (Weapon) {
			// To Handsocket
			Weapon->SetWeaponFlying(this);
		}
	}
}

void AExtendedUnitBase::SetWeaponReturning() {
	if (WeaponClass) {
		if (Weapon) {
			// To Handsocket
			Weapon->SetWeaponReturning(this);
		}
	}
}

void AExtendedUnitBase::Fire()
{
	Weapon->PlayFireAnimation();
}

void AExtendedUnitBase::ReloadWeapon() {
	if (WeaponClass) {
		if (Weapon && MagCount[ActualWeaponId] > 0) {
			// To Handsocket
			MagCount[ActualWeaponId]--;
			MagSize[ActualWeaponId] = MaxMagSize[ActualWeaponId];
		}
	}
}

USkeletalMesh* AExtendedUnitBase::GetWeaponMesh()
{
	if(!Weapon)
		return nullptr;

	return Weapon->GetWeaponMesh(Weapons[ActualWeaponId]);

}


void AExtendedUnitBase::CreateMineSpawnRadius()
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < MineSpawnRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 1.2);
		}
		else if (IconScale.X - 0.5 > MineSpawnRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 0.95);
		}
		SelectedIcon->ChangeMaterialToAction(FVector4d(0.396,0,0.00657,0.521));
	}
}

void AExtendedUnitBase::SpawnScatterMine(AActor* Target, AActor* Attacker)
{

	AExtendedUnitBase* ShootingUnit = Cast<AExtendedUnitBase>(Attacker);
	
	if(ShootingUnit)
	{
		FTransform Transform;
		Transform.SetRotation(FQuat(FRotator::ZeroRotator)); 
		
		const auto MyScatterMine = Cast<AScatterMine>
							(UGameplayStatics::BeginDeferredActorSpawnFromClass
							(this, ScatterMineBaseClass, Transform,  ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (MyScatterMine != nullptr)
		{
			if(ScatterMineSpawnLocation.Z >= GetActorLocation().Z)
				ScatterMineSpawnLocation.Z = GetActorLocation().Z-GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
			
			MyScatterMine->Init(Target, Attacker, ScatterMineSpawnLocation);
			UGameplayStatics::FinishSpawningActor(MyScatterMine, Transform);
			Mana -= MineManaCost;
		}
	}
}

void AExtendedUnitBase::CreateTeleportRadius()
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < TeleportRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 1.2);
		}
		else if (IconScale.X - 0.5 > TeleportRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 0.95);
		}
		SelectedIcon->ChangeMaterialToAction(FVector4d(0.396,0,0.00657,0.521));
	}
}

void AExtendedUnitBase::CreateIconActionRadius()
{
	if (Weapon) {

		if (SelectedIcon) {
			FVector IconScale = SelectedIcon->GetActorScale3D();
			IconScale.X = Range/35+0.5;
			IconScale.Y = Range/35+0.5;
			SelectedIcon->SetActorScale3D(IconScale);
			SelectedIcon->ChangeMaterialToAction(FVector4d(0.396,0,0.00657,0.521));

		}
	}
}


void AExtendedUnitBase::CreateIconThrowRangeRadius()
{
	if (Weapon) {

		if (SelectedIcon) {
			FVector IconScale = SelectedIcon->GetActorScale3D();
			IconScale.X = ThrowRange/35+0.5;
			IconScale.Y = ThrowRange/35+0.5;
			SelectedIcon->SetActorScale3D(IconScale);
			SelectedIcon->ChangeMaterialToAction(FVector4d(0.296,0.256,0.00657,0.521));

		}
	}
}

void AExtendedUnitBase::CreateIconGravityRangeRadius()
{
	if (Weapon) {

		if (SelectedIcon) {
			FVector IconScale = SelectedIcon->GetActorScale3D();
			IconScale.X = GravityRange/35+0.5;
			IconScale.Y = GravityRange/35+0.5;
			SelectedIcon->SetActorScale3D(IconScale);
			SelectedIcon->ChangeMaterialToAction(FVector4d(0.00657,0.256, 0.296,0.521));

		}
	}
}

void AExtendedUnitBase::CreateIconNormalRadius()
{
	if (SelectedIcon) {

		if(ShowGravityRadius)
		{
			CreateIconGravityRangeRadius();
		}else if(ShowThrowRadius)
		{
			CreateIconThrowRangeRadius();
		}else
		{
			FVector IconScale = SelectedIcon->GetActorScale3D();
			if (IconScale.X + 0.5 < 1.0f) {
				SelectedIcon->SetActorScale3D(FVector(1.0f));
			
			}
			else if (IconScale.X - 0.5 > 1.0f) {
				SelectedIcon->SetActorScale3D(FVector(1.0f));
			}

			SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 40.f, 30.f, 0.5f));
		
			if (Shield) {
				if (Shield->IsActive)
					SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 10.f, 100.f, 0.5f));
				else
					SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 40.f, 30.f, 0.5f));
			}
		}
	}
}

void AExtendedUnitBase::ToggleShield()
{
	if (Shield) {
		if (Shield->IsActive) {
			Shield->IsActive = false;
			Shield->Hide();
			HealthWidgetComp->bHiddenInGame = false;
		}
		else if (Shield->HitCounter > 0) {
			Shield->IsActive = true;
			Shield->Show();
			HealthWidgetComp->bHiddenInGame = true;

		}
	}

}

void AExtendedUnitBase::SpawnShield()
{
	if(!ShieldBaseClass)
		return;
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpellTransform;
	SpellTransform.SetLocation(FVector(0, 0, 0));
	SpellTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	Shield = GetWorld()->SpawnActor<AShield>(ShieldBaseClass, SpellTransform, SpawnParams);

	if (Shield)
	{
		USceneComponent* CharacterMesh = Cast<USceneComponent>(GetMesh());
		Shield->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("rootSocket"));
		Shield->SetActorScale3D(FVector(2.0f));
		Shield->SetActorRelativeLocation(FVector(0, 0, 80));
		Shield->IsActive = false;
		Shield->Hide();
	}
}

void AExtendedUnitBase::RestoreMana(float DeltaTime)
{
	ManaTime += DeltaTime;
	if(ManaTime >= ManaRestoreTime)
	{
		ManaTime = 0;
		Mana < MaxMana? Mana += ManaRestore : Mana = MaxMana;
	}
	
}

void AExtendedUnitBase::CreateMouseBotSpawnRadius()
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < MouseBotRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 1.2);
		}
		else if (IconScale.X - 0.5 > MouseBotRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 0.95);
		}
		SelectedIcon->ChangeMaterialToAction(FVector4d(0.396,0,0.00657,0.521));
	}
}

void AExtendedUnitBase::SpawnMouseBot()
{
	if(!MouseBotBaseClass)
		return;

	FTransform Transform;
	Transform.SetLocation(FVector(MouseBotSpawnLocation));
	
	const auto Bot = Cast<AMouseBotBase>
		(UGameplayStatics::BeginDeferredActorSpawnFromClass
		(this, MouseBotBaseClass, Transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	
	UGameplayStatics::FinishSpawningActor(Bot, Transform);
	
	MouseBot = Bot;
	MouseBot->SetAnimState(BotData::Run_Bot);
	Mana -= BotManaCost;
}
