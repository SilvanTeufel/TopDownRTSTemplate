// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Characters/Unit/UnitBase.h"

#include "Actors/IndicatorActor.h"
#include "Widgets/UnitBaseHealthBar.h"
#include "Components/CapsuleComponent.h"
#include "Actors/SelectedIcon.h"
#include "Actors/Weapon/Projectile.h"
#include "Controller/Unit/UnitControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AUnitBase::AUnitBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	
	if (RootComponent == nullptr) {
		RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	}
	
	HealthWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("Healthbar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	Health = MaxHealth;
	
}


// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
	

		SetupHealthWidget();
		FRotator NewRotation = FRotator(0, -90, 0);
		FQuat QuatRotation = FQuat(NewRotation);
		
		GetMesh()->SetRelativeRotation(QuatRotation, false, 0, ETeleportType::None);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -75), false, 0, ETeleportType::None);
		SpawnSelectedIcon();


		CreateSpawnDataFromDataTable();
	
		GetCharacterMovement()->GravityScale = 1;
		
		if(IsFlying)
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			FVector UnitLocation = GetActorLocation();
			SetActorLocation(FVector(UnitLocation.X, UnitLocation.Y, FlyHeight));
		}

		IsHiddenAtStart = false;
		if(IsHiddenAtStart)
		{
			SetActorHiddenInGame(true);
		}

}

void AUnitBase::SetupHealthWidget()
{
	if (HealthWidgetComp) {

		HealthWidgetComp->SetRelativeLocation(HealthWidgetCompLocation, false, 0, ETeleportType::None);
		UUnitBaseHealthBar* Healthbar = Cast<UUnitBaseHealthBar>(HealthWidgetComp->GetUserWidgetObject());

		if (Healthbar) {
			Healthbar->SetOwnerActor(this);
		}
	}
	Health = MaxHealth;
}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// HUDBase related //////////////////////////////////////////////////
void AUnitBase::IsAttacked(AActor* AttackingCharacter) // AActor* SelectedCharacter
{
	SetUnitState(UnitData::IsAttacked);
	SetWalkSpeed(IsAttackedSpeed);

}
void AUnitBase::SetWalkSpeed(float Speed)
{
	UCharacterMovementComponent* MovementPtr = GetCharacterMovement();
	if(Speed == 0.f)
	{
		MovementPtr->StopMovementImmediately();
	}else
	{
		MovementPtr->MaxWalkSpeed = Speed;
	}
}
/////////////////////////////////////////////////////////////////////


void AUnitBase::SetWaypoint(AWaypoint* NewNextWaypoint)
{
	NextWaypoint = NewNextWaypoint;
}

void AUnitBase::SetUnitState(TEnumAsByte<UnitData::EState> NewUnitState)
{
	UnitState = NewUnitState;
}

TEnumAsByte<UnitData::EState> AUnitBase::GetUnitState()
{
	return UnitState;
}

// Enemy_Healthbar related //////////////////////////////////////////////////
float AUnitBase::GetHealth()
{
	return Health;
}

void AUnitBase::SetHealth(float NewHealth)
{
	

	if(NewHealth <= 0.f)
	{
		Health = 0;
		SetWalkSpeed(0);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorEnableCollision(false);
		SetDeselected();
		SpawnSelectablesArray();
		SetUnitState(UnitData::Dead);
		UnitControlTimer = 0.f;
	}else if(NewHealth >= MaxHealth)
	{
		Health = MaxHealth;
	}else
	{
		Health = NewHealth;
	}
}

float AUnitBase::GetMaxHealth()
{
	return MaxHealth;
}

/////////////////////////////////////////////////////////////////////


// HUDBase related //////////////////////////////////////////////////
void AUnitBase::SetSelected()
{
	if(SelectedIcon)
		SelectedIcon->IconMesh->bHiddenInGame = false;

}

void AUnitBase::SetDeselected()
{
	if (SelectedIcon)
	{
		SelectedIcon->IconMesh->bHiddenInGame = true;
		SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 40.f, 30.f, 0.5f));
	}
}


void AUnitBase::SpawnSelectedIcon()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpellTransform;
	SpellTransform.SetLocation(FVector(500, 0, 0));
	SpellTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	SelectedIcon = GetWorld()->SpawnActor<ASelectedIcon>(ASelectedIcon::StaticClass(), SpellTransform, SpawnParams);
	if (SelectedIcon) {
		SelectedIcon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("rootSocket"));
		SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 40.f, 30.f, 0.5f));
	}
}

void AUnitBase::SpawnProjectile(AActor* Target, AActor* Attacker) // FVector TargetLocation
{
	AUnitBase* ShootingUnit = Cast<AUnitBase>(Attacker);

	if(ShootingUnit)
	{
		FTransform Transform;
		Transform.SetLocation(GetActorLocation() + ProjectileScaleActorDirectionOffset*GetActorForwardVector() + ProjectileSpawnOffset);
		Transform.SetRotation(FQuat(FRotator::ZeroRotator)); // FRotator::ZeroRotator
		Transform.SetScale3D(ShootingUnit->ProjectileScale);
		
		const auto MyProjectile = Cast<AProjectile>
							(UGameplayStatics::BeginDeferredActorSpawnFromClass
							(this, ProjectileBaseClass, Transform,  ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (MyProjectile != nullptr)
		{
		
			MyProjectile->Init(Target, Attacker);
			MyProjectile->Mesh->OnComponentBeginOverlap.AddDynamic(MyProjectile, &AProjectile::OnOverlapBegin); 
			UGameplayStatics::FinishSpawningActor(MyProjectile, Transform);
		}
	}
}


void AUnitBase::SpawnIndicator(float Damage, FVector Location) // FVector TargetLocation
{
	
	if(Damage > 0)
	{
		FTransform Transform;
		Transform.SetLocation(Location);
		Transform.SetRotation(FQuat(FRotator::ZeroRotator)); // FRotator::ZeroRotator

		const auto MyIndicator = Cast<AIndicatorActor>
							(UGameplayStatics::BeginDeferredActorSpawnFromClass
							(this, IndicatorBaseClass, Transform,  ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (MyIndicator != nullptr)
		{

			UGameplayStatics::FinishSpawningActor(MyIndicator, Transform);
			MyIndicator->SpawnDamageIndicator(Damage);
		}
	}
}


bool AUnitBase::SetNextUnitToChase()
{
	if(!UnitsToChase.Num()) return false;
		
	float ShortestDistance = GetDistanceTo(UnitsToChase[0]);
	int IndexShortestDistance = 0;
	for(int i = 0; i < UnitsToChase.Num(); i++)
	{
		if(UnitsToChase[i] && UnitsToChase[i]->GetUnitState() != UnitData::Dead)
		{
			DistanceToUnitToChase = GetDistanceTo(UnitsToChase[i]);
						
			if(DistanceToUnitToChase < ShortestDistance)
			{
				ShortestDistance = DistanceToUnitToChase;
				IndexShortestDistance = i;
			}
		}
	}

	bool RValue = false;
	
	if(UnitsToChase[IndexShortestDistance] && UnitsToChase[IndexShortestDistance]->GetUnitState() != UnitData::Dead)
	{
		UnitToChase = UnitsToChase[IndexShortestDistance];
		RValue =  true;
	}

	TArray <AUnitBase*> UnitsToDelete = UnitsToChase;
	
	for(int i = 0; i < UnitsToDelete.Num(); i++)
	{
		if(UnitsToDelete[i])
		{
			float Distance =  FVector::Dist(GetActorLocation(), UnitsToDelete[i]->GetActorLocation());
		
			AUnitControllerBase* UnitController = Cast<AUnitControllerBase>(GetController());

			if(UnitsToDelete[i] && UnitsToDelete[i]->GetUnitState() == UnitData::Dead)
				UnitsToChase.Remove(UnitsToDelete[i]);
			else if(UnitsToDelete[i] && UnitController && Distance > UnitController->LoseSightRadius)
			{
				if(!IsFriendly)
					SetActorHiddenInGame(true);
				UnitsToChase.Remove(UnitsToDelete[i]);
			}
			
		}
	}

	return RValue;
}


/////////////////////////////////////////////////////////////////////

