// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Characters/Unit/TalentedUnit.h"
#include "Actors/Weapon/WeaponBase.h"
#include "Characters/Unit/UnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/UnitTimerWidget.h"

ATalentedUnit::ATalentedUnit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) // Pass initializer to intermediate class
{
	// ATalentedUnit initialization code
	TimerWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("Timer"));
	TimerWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ATalentedUnit::BeginPlay()
{
	Super::BeginPlay();
	SetupTimerWidget();
	if(DisableSaveGame)// DisableSaveGame
	{
		CreateWeaponTalentData();
	}else
	{
		WeaponTalentDataArray = LoadTalentPoints();

		InitShieldMaxHitCounter = Shield->MaxHitCounter;
		SetTalentPoints(ActualWeaponId);
		if(!WeaponTalentDataArray.Num())
		{
			CreateWeaponTalentData();
		}	
	}
}

void ATalentedUnit::CreateWeaponTalentData()
{
	if (WeaponTalentTable)
	{
		// Iterate through the WeaponTalentTable
		TMap<FName, uint8*> RowMap = WeaponTalentTable->GetRowMap();
		for (auto& Pair : RowMap)
		{
			FName RowName = Pair.Key;
			uint8* RowData = Pair.Value;

			// Convert the row data to the appropriate struct
			FWeaponTalents* WeaponTalents = reinterpret_cast<FWeaponTalents*>(RowData);

			// Create a new FWeaponTalentData instance
			FWeaponTalentData TalentData;

			// Assign the WeaponTalents to the Talents property
			TalentData.Talents = *WeaponTalents;

			// Create a new FWeaponTalentPoints instance and initialize its properties
			FWeaponTalentPoints TalentPoints;
			TalentPoints.WeaponId = WeaponTalents->WeaponId;
			TalentPoints.RangeOffset = 0;
			TalentPoints.MagOffset = 0;
			TalentPoints.AttackSpeedScaler = 0;
			TalentPoints.PiercedTargetsOffset = 0;
			TalentPoints.ProjectileScaler = 0;
			TalentPoints.ProjectileSpeedScaler = 0;
			TalentPoints.WeaponStunOffset = 0;
			TalentPoints.LaserCountOffset = 0;
			TalentPoints.ShieldCountOffset = 0;
			// Assign other properties similarly

			// Assign the TalentPoints to the TalentPoints property
			TalentData.TalentPoints = TalentPoints;
			WeaponTalentDataArray.Add(TalentData);


			// Add the TalentData to the WeaponTalentDataArray
	
			
		
			AvailableTalentPoints.Add(MaxAvailableTalentPoints);
		}
	}
	//return 1;
}


void ATalentedUnit::IncrementTalentPoint(int32 WeaponId, const FString& TalentKey)
{
	// Iterate through the WeaponTalentDataArray
	if(WeaponTalentDataArray.Num())
	for (FWeaponTalentData& TalentData : WeaponTalentDataArray)
	{
		// Check if the WeaponId matches
		if (TalentData.Talents.WeaponId == WeaponId && AvailableTalentPoints.Num() && AvailableTalentPoints[WeaponId] > 0)
		{
			// Check the talent key and increment the corresponding talent point
			if (TalentKey == "RangeOffset" && TalentData.TalentPoints.RangeOffset < MaxTalentPoints)
			{
				TalentData.TalentPoints.RangeOffset += 1;
				AvailableTalentPoints[WeaponId]-=1;
				Range = Range +TalentData.Talents.RangeOffset;
			}
			else if (TalentKey == "MagOffset" && TalentData.TalentPoints.MagOffset < MaxTalentPoints)
			{
				TalentData.TalentPoints.MagOffset += 1;
				AvailableTalentPoints[WeaponId]-=1;
				MaxMagSize[WeaponId] = MaxMagSize[WeaponId]+TalentData.Talents.MagOffset;
			}
			else if (TalentKey == "AttackSpeedScaler" && TalentData.TalentPoints.AttackSpeedScaler < MaxTalentPoints)
			{
				TalentData.TalentPoints.AttackSpeedScaler += 1;
				AvailableTalentPoints[WeaponId]-=1;
				AttackDuration = AttackDuration*TalentData.Talents.AttackSpeedScaler;
				ReloadTime = ReloadTime*TalentData.Talents.AttackSpeedScaler;
				PauseDuration = PauseDuration*TalentData.Talents.AttackSpeedScaler;
			}
			else if (TalentKey == "PiercedTargetsOffset" && TalentData.TalentPoints.PiercedTargetsOffset < MaxTalentPoints)
			{
				TalentData.TalentPoints.PiercedTargetsOffset += 1;
				AvailableTalentPoints[WeaponId]-=1;
				MaxPiercedTargets = MaxPiercedTargets +TalentData.Talents.PiercedTargetsOffset;
			}
			else if (TalentKey == "ProjectileScaler" && TalentData.TalentPoints.ProjectileScaler < MaxTalentPoints)
			{
				TalentData.TalentPoints.ProjectileScaler += 1;
				AvailableTalentPoints[WeaponId]-=1;
				ProjectileScale = ProjectileScale*TalentData.Talents.ProjectileScaler;
			}
			else if (TalentKey == "ProjectileSpeedScaler" && TalentData.TalentPoints.ProjectileSpeedScaler < MaxTalentPoints)
			{
				TalentData.TalentPoints.ProjectileSpeedScaler += 1;
				AvailableTalentPoints[WeaponId]-=1;
				ProjectileSpeed = ProjectileSpeed*TalentData.Talents.ProjectileSpeedScaler;
			}else if(TalentKey == "WeaponStunOffset" && TalentData.TalentPoints.WeaponStunOffset < MaxTalentPoints)
			{
				TalentData.TalentPoints.WeaponStunOffset += 1;
				AvailableTalentPoints[WeaponId]-=1;
				TalentWeaponStunOffset = TalentWeaponStunOffset +TalentData.Talents.WeaponStunOffset;
			}else if(TalentKey == "LaserCountOffset" && TalentData.TalentPoints.LaserCountOffset < MaxTalentPoints){
				TalentData.TalentPoints.LaserCountOffset += 1;
				AvailableTalentPoints[WeaponId]-=1;
				LaserCount = LaserCount+TalentData.Talents.LaserCountOffset;
			}else if(TalentKey == "ShieldCountOffset" && TalentData.TalentPoints.ShieldCountOffset < MaxTalentPoints){
				TalentData.TalentPoints.ShieldCountOffset += 1;
				AvailableTalentPoints[WeaponId]-=1;
				Shield->MaxHitCounter = Shield->MaxHitCounter +TalentData.Talents.ShieldCountOffset;
			}else
			{
				// Handle unsupported talent key
			}

			// AttackDamage = Weapon->GetWeaponDamage(Weapons[ActualWeaponId]);
			// Break the loop since we found and updated the talent point
			break;
		}
	}
	SaveTalentPoints(WeaponTalentDataArray);
}

void ATalentedUnit::SetTalentPoints(int32 WeaponId)
{
	// Iterate through the WeaponTalentDataArray

	if(WeaponTalentDataArray.Num())
	for (FWeaponTalentData& TalentData : WeaponTalentDataArray)
	{
		// Check if the WeaponId matches
		
		if (TalentData.Talents.WeaponId == WeaponId )
		{
			// Check the talent key and increment the corresponding talent point
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
			MaxMagSize[WeaponId] = Weapon->GetWeaponMagSize(Weapons[ActualWeaponId]);
			
			Range = Range+TalentData.Talents.RangeOffset*TalentData.TalentPoints.RangeOffset;
			MaxMagSize[WeaponId] = MaxMagSize[WeaponId]+(TalentData.Talents.MagOffset*TalentData.TalentPoints.MagOffset);
			MaxPiercedTargets = MaxPiercedTargets+(TalentData.Talents.PiercedTargetsOffset*TalentData.TalentPoints.PiercedTargetsOffset);

			AttackDuration = AttackDuration * pow(TalentData.Talents.AttackSpeedScaler, TalentData.TalentPoints.AttackSpeedScaler);
			ReloadTime = ReloadTime * pow(TalentData.Talents.AttackSpeedScaler, TalentData.TalentPoints.AttackSpeedScaler);
			PauseDuration =  PauseDuration * pow(TalentData.Talents.AttackSpeedScaler, TalentData.TalentPoints.AttackSpeedScaler);
			ProjectileScale = ProjectileScale * pow(TalentData.Talents.ProjectileScaler, TalentData.TalentPoints.ProjectileScaler);
			ProjectileSpeed = ProjectileSpeed * pow(TalentData.Talents.ProjectileSpeedScaler, TalentData.TalentPoints.ProjectileSpeedScaler);

			TalentWeaponStunOffset = InitTalentWeaponStunOffset+(TalentData.Talents.WeaponStunOffset*TalentData.TalentPoints.WeaponStunOffset);
			LaserCount = InitLaserCount+(TalentData.Talents.LaserCountOffset*TalentData.TalentPoints.LaserCountOffset);
			Shield->MaxHitCounter = InitShieldMaxHitCounter+(TalentData.Talents.ShieldCountOffset*TalentData.TalentPoints.ShieldCountOffset);
			
			break;
		}
	}

	SaveTalentPoints(WeaponTalentDataArray);
}

void ATalentedUnit::ResetTalentPoint(int32 WeaponId)
{
	// Iterate through the WeaponTalentDataArray
	for (FWeaponTalentData& TalentData : WeaponTalentDataArray)
	{
		// Check if the WeaponId matches
		if (TalentData.Talents.WeaponId == WeaponId && AvailableTalentPoints.Num())
		{
				TalentData.TalentPoints.RangeOffset = 0;
				TalentData.TalentPoints.MagOffset = 0;
				TalentData.TalentPoints.AttackSpeedScaler  = 0;
				TalentData.TalentPoints.PiercedTargetsOffset = 0;
				TalentData.TalentPoints.ProjectileScaler = 0;
				TalentData.TalentPoints.ProjectileSpeedScaler  = 0;
				TalentData.TalentPoints.WeaponStunOffset = 0;
				TalentData.TalentPoints.LaserCountOffset = 0;
				TalentData.TalentPoints.ShieldCountOffset = 0;
				TalentWeaponStunOffset = InitTalentWeaponStunOffset;
				LaserCount = InitLaserCount;
				Shield->MaxHitCounter = InitShieldMaxHitCounter;
			

			if (WeaponClass)
			{
				if (Weapon) {
					// To Handsocket
					Weapon->ChangeWeapon(Weapons[ActualWeaponId]);

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
					MaxMagSize[WeaponId] = Weapon->GetWeaponMagSize(Weapons[ActualWeaponId]);
				}
			}
				AvailableTalentPoints[WeaponId] = MaxAvailableTalentPoints;
				// Handle unsupported talent key
				
			break;
		}

			// AttackDamage = Weapon->GetWeaponDamage(Weapons[ActualWeaponId]);
			// Break the loop since we found and updated the talent point

	}
	SaveTalentPoints(WeaponTalentDataArray);
}


void ATalentedUnit::AddTalentPoint(int Amount)
{
	for (int& TalentPoint : AvailableTalentPoints)
	{
		TalentPoint += Amount;
	}
}

void ATalentedUnit::SaveTalentPoints(const TArray<FWeaponTalentData>& TalentDataArray)
{
	UTalentSaveGame* SaveGameInstance = Cast<UTalentSaveGame>(UGameplayStatics::CreateSaveGameObject(UTalentSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		SaveGameInstance->WeaponTalentDataArray = TalentDataArray;
		SaveGameInstance->AvailableTalentPoints = AvailableTalentPoints;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("TalentsSave0100"), 0);
	}
}

TArray<FWeaponTalentData> ATalentedUnit::LoadTalentPoints()
{
	UTalentSaveGame* SaveGameInstance = Cast<UTalentSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("TalentsSave0100"), 0));

	if (SaveGameInstance)
	{
		AvailableTalentPoints = SaveGameInstance->AvailableTalentPoints;
		return SaveGameInstance->WeaponTalentDataArray;
	}

	// Return an empty array if no save game exists
	return TArray<FWeaponTalentData>();
}

void ATalentedUnit::SetupTimerWidget()
{
	if (TimerWidgetComp) {

		TimerWidgetComp->SetRelativeLocation(TimerWidgetCompLocation, false, 0, ETeleportType::None);
		UUnitTimerWidget* Timerbar = Cast<UUnitTimerWidget>(TimerWidgetComp->GetUserWidgetObject());

		if (Timerbar) {
			Timerbar->SetOwnerActor(this);
		}
	}
}
