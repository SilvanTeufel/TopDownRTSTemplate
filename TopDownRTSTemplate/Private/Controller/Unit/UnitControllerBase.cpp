// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Controller/Unit/UnitControllerBase.h"
#include <string>
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Actors/Waypoint.h"
#include "Actors/Weapon/WeaponBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameSession.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Perception/AiPerceptionComponent.h"
#include "Perception/AiSenseConfig_Sight.h"
#include "Components/SkeletalMeshComponent.h"

AUnitControllerBase::AUnitControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfig->SetMaxAge(SightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

}

void AUnitControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	UAIPerceptionComponent* PComponent = GetPerceptionComponent();
	if (PComponent)
	{
		if (SightConfig)
		{
			PComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
			PComponent->OnPerceptionUpdated.AddDynamic(this, &AUnitControllerBase::OnUnitDetected);
			//PComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AUnitControllerBase::OnTargetPerceptionUpdated);
			PComponent->ConfigureSense(*SightConfig);
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("SightConfig is null. Check that it's properly assigned."));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("PerceptionComponent is null. Make sure it exists."));
	}
}

void AUnitControllerBase::OnPossess(APawn* PawN)
{
	Super::OnPossess(PawN);
}



void AUnitControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UnitControlStateMachine(DeltaSeconds);
}

void AUnitControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AUnitBase* CurrentUnit = Cast<AUnitBase>(GetPawn());
	
	if (Stimulus.WasSuccessfullySensed())
	{
		// Unit is perceived, add it to the set of perceived units
		AUnitBase* DetectedUnit = Cast<AUnitBase>(Actor);
		if (DetectedUnit == nullptr)
			return;

		if (!DetectFriendlyUnits && DetectedUnit && CurrentUnit && (DetectedUnit->IsFriendly != CurrentUnit->IsFriendly))
		{
	
			TEnumAsByte<UnitData::EState> CurrentUnitState = CurrentUnit->GetUnitState();
			if (CurrentUnitState != UnitData::Dead &&
				CurrentUnitState != UnitData::HoldPositionCreate &&
				CurrentUnitState != UnitData::HoldPositionWatch &&
				CurrentUnitState != UnitData::HoldPositionAttack &&
				CurrentUnitState != UnitData::HoldPositionPause)
			{
	
				DistanceToUnitToChase = GetPawn()->GetDistanceTo(DetectedUnit);
	
				DetectedUnit->SetActorHiddenInGame(false);
                				
				CurrentUnit->UnitsToChase.Emplace(DetectedUnit);
				CurrentUnit->SetNextUnitToChase();

				if (CurrentUnit->UnitToChase)
				{
					if (CurrentUnit->GetUnitState() != UnitData::Run)
					{
						CurrentUnit->AddMovementInput(FVector(0.f), CurrentUnit->RunSpeedScale);

						if (CurrentUnit->ToggleUnitDetection)
							CurrentUnit->SetUnitState(UnitData::Chase);
					}
				}
			}
		}
		else if (DetectFriendlyUnits && DetectedUnit && CurrentUnit && (DetectedUnit->IsFriendly == CurrentUnit->IsFriendly))
		{
			if (DetectedUnit->GetUnitState() != UnitData::Dead && CurrentUnit->GetUnitState() != UnitData::Dead)
			{
				DistanceToUnitToChase = GetPawn()->GetDistanceTo(DetectedUnit);

				CurrentUnit->UnitsToChase.Emplace(DetectedUnit);
				CurrentUnit->SetNextUnitToChase();

				if (CurrentUnit->UnitToChase)
				{
					if (CurrentUnit->GetUnitState() != UnitData::Attack && CurrentUnit->GetUnitState() != UnitData::Run && CurrentUnit->UnitToChase->GetHealth() < CurrentUnit->UnitToChase->GetMaxHealth())
					{
						CurrentUnit->AddMovementInput(FVector(0.f), CurrentUnit->RunSpeedScale);
						CurrentUnit->SetUnitState(UnitData::Chase);
					}
				}
			}
		}
	}
	else
	{
		AUnitBase* DetectedUnit = Cast<AUnitBase>(Actor);
		DetectedUnit->SetActorHiddenInGame(true);
	
	}
}

void AUnitControllerBase::OnUnitDetected(const TArray<AActor*>& DetectedUnits)
{
	AUnitBase* CurrentUnit = Cast<AUnitBase>(GetPawn());

	/*
	for (AActor* DetectedActor : DetectedUnits)
	{
		AUnitBase* DetectedUnit = Cast<AUnitBase>(DetectedActor);
		if(DetectedUnit && !DetectedUnit->IsFriendly && (DetectedUnit->IsFriendly != CurrentUnit->IsFriendly))
		{
			DetectedUnit->SetActorHiddenInGame(false);
		}
	}*/
	
	for (AActor* DetectedActor : DetectedUnits)
	{
		AUnitBase* DetectedUnit = Cast<AUnitBase>(DetectedActor);
		if (DetectedUnit == nullptr)
		{
			continue;
		}

		if (!DetectFriendlyUnits && DetectedUnit && CurrentUnit && (DetectedUnit->IsFriendly != CurrentUnit->IsFriendly))
		{
			
			TEnumAsByte<UnitData::EState> CurrentUnitState = CurrentUnit->GetUnitState();
			if (CurrentUnitState != UnitData::Dead &&
				CurrentUnitState != UnitData::HoldPositionCreate &&
				CurrentUnitState != UnitData::HoldPositionWatch &&
				CurrentUnitState != UnitData::HoldPositionAttack &&
				CurrentUnitState != UnitData::HoldPositionPause)
			{
		
				DistanceToUnitToChase = GetPawn()->GetDistanceTo(DetectedUnit);
                				
				CurrentUnit->UnitsToChase.Emplace(DetectedUnit);
				CurrentUnit->SetNextUnitToChase();

				if (CurrentUnit->UnitToChase)
				{
					if (CurrentUnit->GetUnitState() != UnitData::Run)
					{
						CurrentUnit->AddMovementInput(FVector(0.f), CurrentUnit->RunSpeedScale);

						if (CurrentUnit->ToggleUnitDetection)
							CurrentUnit->SetUnitState(UnitData::Chase);
					}
				}
			}
		}
		else if (DetectFriendlyUnits && DetectedUnit && CurrentUnit && (DetectedUnit->IsFriendly == CurrentUnit->IsFriendly))
		{
			if (DetectedUnit->GetUnitState() != UnitData::Dead && CurrentUnit->GetUnitState() != UnitData::Dead)
			{
				DistanceToUnitToChase = GetPawn()->GetDistanceTo(DetectedUnit);

				CurrentUnit->UnitsToChase.Emplace(DetectedUnit);
				CurrentUnit->SetNextUnitToChase();

				if (CurrentUnit->UnitToChase)
				{
					if (CurrentUnit->GetUnitState() != UnitData::Attack && CurrentUnit->GetUnitState() != UnitData::Run && CurrentUnit->UnitToChase->GetHealth() < CurrentUnit->UnitToChase->GetMaxHealth())
					{
						CurrentUnit->AddMovementInput(FVector(0.f), CurrentUnit->RunSpeedScale);
						CurrentUnit->SetUnitState(UnitData::Chase);
					}
				}
			}
		}
	}
}

FRotator AUnitControllerBase::GetControlRotation() const
{
	if (GetPawn() == nullptr) {
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AUnitControllerBase::KillUnitBase(AUnitBase* UnitBase)
{
	UnitBase->SetWalkSpeed(0);
	UnitBase->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UnitBase->SetActorEnableCollision(false);
	UnitBase->SetDeselected();
	UnitBase->SetUnitState(UnitData::Dead);
}



/*
void AUnitControllerBase::OnUnitDetected(const TArray<AActor*>& DetectedUnits)
{
	AUnitBase* CurrentUnit = Cast<AUnitBase>(GetPawn());
	AUnitBase* DetectedUnit = Cast<AUnitBase>(DetectedUnits[0]);
	
	//if(DetectedUnit) 	DetectedUnit->SetActorHiddenInGame(false);
		// Linetrace here
	
	if(!DetectFriendlyUnits && DetectedUnit && CurrentUnit && (DetectedUnit->IsFriendly != CurrentUnit->IsFriendly)) 
	{
		TEnumAsByte<UnitData::EState> CurrentUnitState = CurrentUnit->GetUnitState();
		if(DetectedUnit->GetUnitState() != UnitData::Dead &&
			CurrentUnitState != UnitData::Dead &&
			CurrentUnitState != UnitData::HoldPositionCreate &&
			CurrentUnitState != UnitData::HoldPositionWatch &&
			CurrentUnitState != UnitData::HoldPositionAttack &&
			CurrentUnitState != UnitData::HoldPositionPause)
		{
			DistanceToUnitToChase = GetPawn()->GetDistanceTo(DetectedUnit);
			
				CurrentUnit->UnitsToChase.Emplace(DetectedUnit);
				CurrentUnit->SetNextUnitToChase();
				
				if (CurrentUnit->UnitToChase) {
					if(CurrentUnit->GetUnitState() != UnitData::Run)
					{
						CurrentUnit->AddMovementInput(FVector(0.f), CurrentUnit->RunSpeedScale);

						if(CurrentUnit->ToggleUnitDetection)
						CurrentUnit->SetUnitState(UnitData::Chase);
					}
				}
			
		}
	}else if (DetectFriendlyUnits && DetectedUnit && CurrentUnit && (DetectedUnit->IsFriendly == CurrentUnit->IsFriendly)) {
		if(DetectedUnit->GetUnitState() != UnitData::Dead && CurrentUnit->GetUnitState() != UnitData::Dead)
		{
			DistanceToUnitToChase = GetPawn()->GetDistanceTo(DetectedUnit);
			
			CurrentUnit->UnitsToChase.Emplace(DetectedUnit);
			CurrentUnit->SetNextUnitToChase();
				
			if (CurrentUnit->UnitToChase) {
				if(CurrentUnit->GetUnitState() != UnitData::Attack && CurrentUnit->GetUnitState() != UnitData::Run && CurrentUnit->UnitToChase->GetHealth() < CurrentUnit->UnitToChase->GetMaxHealth())
				{
					CurrentUnit->AddMovementInput(FVector(0.f), CurrentUnit->RunSpeedScale);
					CurrentUnit->SetUnitState(UnitData::Chase);
				}
			}
			
		}
	}
}*/

void AUnitControllerBase::RotateToAttackUnit(AUnitBase* AttackingUnit, AUnitBase* UnitToAttack)
{
	if(AttackingUnit && UnitToAttack)
	{

		//float Distance = FVector::Distance(AttackingUnit->GetActorLocation(), UnitToAttack->GetActorLocation());
		FVector AttackerLocation = AttackingUnit->GetActorLocation();
		FVector IsAttackedLocation = UnitToAttack->GetActorLocation();
		FVector ForwardVector = AttackingUnit->GetActorForwardVector();
		float AngleDiff = CalculateAngle(ForwardVector, FVector(IsAttackedLocation.X, IsAttackedLocation.Y, ForwardVector.Z) - FVector(AttackerLocation.X, AttackerLocation.Y, ForwardVector.Z));
		if(FMath::Abs(AngleDiff) < 0.01f) return;
		//float DistanceMultiplier = FMath::Clamp(FMath::Pow(Distance / 500.f, 4.f), 0.f, 1.f);
		float PreRotator = (FMath::Pow(AngleDiff/ 20.0f, 4));
	
		float Inverter = AngleDiff < 0.f ? -1.f : 1.f;

		float RotationValue = FMath::Clamp(PreRotator *Inverter, -50.0f, 50.0f);
	
		FQuat QuadRotation = FQuat(FRotator(0.f, RotationValue, 0.f));
		AttackingUnit->AddActorLocalRotation(QuadRotation, false, 0, ETeleportType::None);
	}
}

void AUnitControllerBase::UnitControlStateMachine(float DeltaSeconds)
{
	AUnitBase* UnitBase = Cast<AUnitBase>(GetPawn());
	
		switch (UnitBase->UnitState)
		{
		case UnitData::None:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("None"));
		}
		break;
		case UnitData::Dead:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Dead"));
			Dead(UnitBase, DeltaSeconds);
		}
		break;
		case UnitData::Patrol:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Patrol"));
			Patrol(UnitBase, DeltaSeconds);
		}
		break;
		case UnitData::Run:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Run"));
			Run(UnitBase, DeltaSeconds);
				
		}
		break;
		case UnitData::Chase:
		{
			//if(!UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Chase"));
			Chase(UnitBase, DeltaSeconds);
		}
		break;
		case UnitData::Attack:
		{
			//if(!UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Attack"));
			Attack(UnitBase, DeltaSeconds);
		}
		break;
		case UnitData::Pause:
		{
				
			//if(!UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Pause"));
			Pause(UnitBase, DeltaSeconds);
		}
		break;
		case UnitData::IsAttacked:
		{
			//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("IsAttacked"));
			IsAttacked(UnitBase, DeltaSeconds);

		}
		break;
		case UnitData::Stunned:
			{
				//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("IsAttacked"));
				IsStunned(UnitBase, DeltaSeconds);

			}
			break;
		case UnitData::Idle:
			{
				//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT("Idle"));
				Idle(UnitBase, DeltaSeconds);
			}
		break;
		default:
			{
				//UnitBase->SetUnitState(UnitData::Idle);
			}
		break;
		}

	if (UnitBase->GetHealth() <= 0.f && UnitBase->GetUnitState() != UnitData::Dead) {
		KillUnitBase(UnitBase);
		UnitBase->UnitControlTimer = 0.f;
	}
}

void AUnitControllerBase::Dead(AUnitBase* UnitBase, float DeltaSeconds)
{

	UnitBase->SetWalkSpeed(0);
				
	UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);

	FVector ActorLocation = UnitBase->GetActorLocation();
				
	UnitBase->SetActorLocation(FVector(ActorLocation.X + 0.f,ActorLocation.Y + 0.f,ActorLocation.Z -1.f));

	if (UnitBase->UnitControlTimer >= DespawnTime) {
		if(UnitBase->DestroyAfterDeath) UnitBase->Destroy(true, false);
	}
}

void AUnitControllerBase::Patrol(AUnitBase* UnitBase, float DeltaSeconds)
{

	UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
				
	if(UnitBase->UnitToChase && UnitBase->UnitToChase->GetUnitState() != UnitData::Dead)
	{
		if(UnitBase->SetNextUnitToChase())
			UnitBase->SetUnitState(UnitData::Chase);
		
	} else if (UnitBase->NextWaypoint != nullptr)
	{

		FVector WaypointLocation = UnitBase->NextWaypoint->GetActorLocation();

		if(UnitBase->IsFlying)
		{
			WaypointLocation =  FVector(WaypointLocation.X, WaypointLocation.Y, UnitBase->FlyHeight);
		}

		if(UnitBase->FollowPath)
		{
			const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitBase->GetActorLocation(), UnitBase->RunLocation);
			UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);
		}else
		{
			const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitBase->GetActorLocation(), WaypointLocation);
			UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);
		}
	}
	else
	{
		UnitBase->SetUnitState(UnitData::Idle);
	}
}

void AUnitControllerBase::Run(AUnitBase* UnitBase, float DeltaSeconds)
{
	
	if(UnitBase->ToggleUnitDetection && UnitBase->UnitToChase)
	{
		if(UnitBase->SetNextUnitToChase())
		{
			UnitBase->SetUnitState(UnitData::Chase);
		}
	}
				
	UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
				
	const FVector UnitLocation = UnitBase->GetActorLocation();
				
	if(UnitBase->IsFlying)
	{
		UnitBase->RunLocation = FVector(UnitBase->RunLocation.X, UnitBase->RunLocation.Y, UnitBase->FlyHeight);
	}
	
	const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitLocation, UnitBase->RunLocation);
	UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);

	const float Distance = sqrt((UnitLocation.X-UnitBase->RunLocation.X)*(UnitLocation.X-UnitBase->RunLocation.X)+(UnitLocation.Y-UnitBase->RunLocation.Y)*(UnitLocation.Y-UnitBase->RunLocation.Y));

	if (Distance <= UnitBase->StopRunTolerance) {
		UnitBase->SetUnitState(UnitData::Idle);
	}
	UnitBase->UnitControlTimer = 0.f;
}

void AUnitControllerBase::Chase(AUnitBase* UnitBase, float DeltaSeconds)
{
				UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);
				
				if (UnitBase->UnitToChase) {
				UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
					
				RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
				DistanceToUnitToChase = GetPawn()->GetDistanceTo(UnitBase->UnitToChase);

					AUnitBase* UnitToChase = UnitBase->UnitToChase;
					
					if (IsUnitToChaseInRange(UnitBase)) { // + RangeOffset
				
						if(UnitBase->UnitControlTimer >= UnitBase->PauseDuration)
						{
							AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(UnitBase);
							if(ExtendedUnitBase)
							{
								// Check if there is still something inside the Magazine
								if(ExtendedUnitBase->MagSize[ExtendedUnitBase->ActualWeaponId] > 0 && !ExtendedUnitBase->UnitDidThrowWeapon)
								{
									// Decrease Magsize by 1 and Attack
									UnitBase->SetUnitState(UnitData::Attack);
									CreateProjectile(UnitBase, UnitBase->UnitToChase);
								}else
								{
									UnitBase->SetUnitState(UnitData::Idle);
								}
							}else
							{
								// Normal UnitBase has no Magazine
								UnitBase->SetUnitState(UnitData::Attack);
								CreateProjectile(UnitBase, UnitBase->UnitToChase);
							}
						}else
						{
							UnitBase->SetUnitState(UnitData::Pause);
						}	
						
					}
					else {


						FVector UnitToChaseLocation = UnitToChase->GetActorLocation();

						if(UnitToChase->IsFlying && !UnitBase->IsFlying)
						{
							if (DistanceToUnitToChase > UnitBase->StopRunToleranceForFlying)
							{
								UnitBase->SetUnitState(UnitData::Idle);
							}
						}
						
						if(UnitBase->IsFlying)
						{
							UnitToChaseLocation =  FVector(UnitToChaseLocation.X, UnitToChaseLocation.Y, UnitBase->FlyHeight);
						}
						const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(UnitBase->GetActorLocation(), UnitToChaseLocation);
						
						UnitBase->AddMovementInput(ADirection, UnitBase->RunSpeedScale);
					}

					if (DistanceToUnitToChase > LoseSightRadius) {
						UnitBase->UnitsToChase.Remove(UnitBase->UnitToChase );
						UnitBase->UnitToChase = nullptr;
						UnitBase->SetUnitState(UnitBase->UnitStatePlaceholder);
					}
				}else
				{
					UnitBase->SetUnitState(UnitBase->UnitStatePlaceholder);
				}
}

bool AUnitControllerBase::IsUnitToChaseInRange(AUnitBase* UnitBase)
{
	if(!UnitBase->UnitToChase) return false;
	if(UnitBase->UnitToChase && UnitBase->UnitToChase->GetUnitState() == UnitData::Dead) return false;
	
	FVector UnitToChaseOrigin;
	FVector UnitToChaseBoxExtent;
	DistanceToUnitToChase = GetPawn()->GetDistanceTo(UnitBase->UnitToChase);
	
	UnitBase->UnitToChase->GetActorBounds(false, UnitToChaseOrigin, UnitToChaseBoxExtent);
	const float RangeOffset = UnitToChaseBoxExtent.X > UnitToChaseBoxExtent.Y? UnitToChaseBoxExtent.X/1.75f : UnitToChaseBoxExtent.Y/1.75f;
	
	if (DistanceToUnitToChase < (UnitBase->Range + RangeOffset))
		return true;
	
	return false;
}

void AUnitControllerBase::Attack(AUnitBase* UnitBase, float DeltaSeconds)
{
	if(!UnitBase) return; 
	
	RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
				
				
	UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);
				
	if(!UnitBase->UnitToChase || UnitBase->UnitToChase->GetUnitState() == UnitData::Dead)
	{
		if(UnitBase->SetNextUnitToChase()) return;
				
		UnitBase->SetUnitState( UnitBase->UnitStatePlaceholder );
		return;
	}

	//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT(" AttackDuration // UnitControlTimer %f // %f"),  UnitBase->AttackDuration, UnitBase->UnitControlTimer);	
				
	if (UnitBase->UnitControlTimer > (UnitBase->AttackDuration + UnitBase->PauseDuration)) {
		if(!UnitBase->UseProjectile )
		{
			// Attack without Projectile
			UnitBase->UnitToChase->SetHealth(UnitBase->UnitToChase->GetHealth()-UnitBase->AttackDamage);

			const TEnumAsByte<UnitData::EState> State = UnitBase->UnitToChase->GetUnitState();
					
			if(State!= UnitData::Run &&
				 State != UnitData::PrepareScatterMine &&
				 State != UnitData::PrepareMouseBot &&
				 State != UnitData::TeleportStart &&
				 State!= UnitData::TeleportEnd &&
				 State != UnitData::Reload )
			{
				UnitBase->UnitToChase->SetUnitState( UnitData::IsAttacked );
			}
			// Attack without Projectile	
		}
		ProjectileSpawned = false;	
		UnitBase->UnitControlTimer = 0.f;
		UnitBase->SetUnitState( UnitData::Pause);
				
	}
}

void AUnitControllerBase::Pause(AUnitBase* UnitBase, float DeltaSeconds)
{
	if(!UnitBase) return; 
	
	UnitBase->SetWalkSpeed(0);
	RotateToAttackUnit(UnitBase, UnitBase->UnitToChase);
				
	UnitBase->UnitControlTimer = (UnitBase->UnitControlTimer + DeltaSeconds);
	
	
	if(!UnitBase->UnitToChase || UnitBase->UnitToChase->GetUnitState() == UnitData::Dead) {

		if(UnitBase->SetNextUnitToChase()) return;
				
		UnitBase->SetUnitState( UnitBase->UnitStatePlaceholder );
				
	} else if (UnitBase->UnitControlTimer > UnitBase->PauseDuration ) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(UnitBase);
		
		if(ExtendedUnitBase && (ExtendedUnitBase->MagSize[ExtendedUnitBase->ActualWeaponId] <= 0 || ExtendedUnitBase->UnitDidThrowWeapon))
		{
			UnitBase->SetUnitState( UnitData::Idle );
			return;
		}
		
		ProjectileSpawned = false;
		UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
		
		if (IsUnitToChaseInRange(UnitBase)) {
				UnitBase->SetUnitState(UnitData::Attack);
				CreateProjectile(UnitBase, UnitBase->UnitToChase);
		}else
		{
			UnitBase->SetUnitState(UnitData::Chase);
		}
		
	}
}

void AUnitControllerBase::IsAttacked(AUnitBase* UnitBase, float DeltaSeconds)
{
	UnitBase->UnitControlTimer += DeltaSeconds;
				
	if (UnitBase->UnitControlTimer > IsAttackedDuration) {
		UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
		if(UnitBase->ToggleUnitDetection)
			UnitBase->SetUnitState(UnitData::Pause);
		else
			UnitBase->SetUnitState(UnitData::Idle);

	}
}

void AUnitControllerBase::IsStunned(AUnitBase* UnitBase, float DeltaSeconds)
{
	UnitBase->UnitControlTimer += DeltaSeconds;
				
	if (UnitBase->UnitControlTimer > (IsStunnedDuration+UnitBase->StunOffset)) {
		UnitBase->SetWalkSpeed(UnitBase->MaxRunSpeed);
		if(UnitBase->ToggleUnitDetection)
			UnitBase->SetUnitState(UnitData::Pause);
		else
			UnitBase->SetUnitState(UnitData::Idle);
	}
}
void AUnitControllerBase::Idle(AUnitBase* UnitBase, float DeltaSeconds)
{

	AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(UnitBase);
		
	if(ExtendedUnitBase && (ExtendedUnitBase->MagSize[ExtendedUnitBase->ActualWeaponId] <= 0 || ExtendedUnitBase->UnitDidThrowWeapon))
	{
		return;
	}
	
	if(UnitBase->SetNextUnitToChase() && UnitBase->UnitsToChase.Num() && UnitBase->ToggleUnitDetection)
	{
		UnitBase->SetUnitState(UnitData::Chase);
	}
}

void AUnitControllerBase::CreateProjectile(AUnitBase* UnitBase, AUnitBase* Target)
{
	if(UnitBase->UseProjectile && !ProjectileSpawned) // This is not 0.f now!
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(UnitBase);

		if(!ExtendedUnitBase && !UnitBase)
			return;
		
		if(ExtendedUnitBase && ExtendedUnitBase->Weapon->GetWeaponMagSize(ExtendedUnitBase->Weapons[ExtendedUnitBase->ActualWeaponId]) < 0)
			return;
		
		if(ExtendedUnitBase)
		{
			ExtendedUnitBase->Fire();
		}

	
		UnitBase->SpawnProjectile(Target, UnitBase);
		
		ProjectileSpawned = true;
	}
}
