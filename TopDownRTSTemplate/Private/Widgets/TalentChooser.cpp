// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TalentChooser.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

#include "Widgets/SpeechBubble.h"

void UTalentChooser::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerUnitBase.IsValid())
		return;

	CreateProgressBars();
	
}

void UTalentChooser::CreateProgressBars()
{
	int WId = OwnerUnitBase->ActualWeaponId;
	
	if (!RangeOffset)
		return;
	if (!MagOffset)
		return;
	if (!AttackSpeedScaler)
		return;
	if (!PiercedTargetsOffset)
		return;
	if (!ProjectileScaler)
		return;
	if (!ProjectileSpeedScaler)
		return;
	if(!AvailableTalents)
		return;

	if(!WeaponStunOffset)
		return;
	if(!LaserCountOffset)
		return;
	if(!LaserCountOffsetButton)
		return;
	if(!ShieldCountOffset)
		return;


	
	void AddWeaponStunOffset();
	
	
	for (const FWeaponTalentData& TalentData : OwnerUnitBase->WeaponTalentDataArray)
	{
		// Create a new UProgressBar widget
		if(TalentData.TalentPoints.WeaponId == WId)
		{
			RangeOffset->SetPercent( float(TalentData.TalentPoints.RangeOffset)/float(OwnerUnitBase->MaxTalentPoints));
			MagOffset->SetPercent( float(TalentData.TalentPoints.MagOffset)/float(OwnerUnitBase->MaxTalentPoints));
			AttackSpeedScaler->SetPercent( float(TalentData.TalentPoints.AttackSpeedScaler)/float(OwnerUnitBase->MaxTalentPoints));
			PiercedTargetsOffset->SetPercent( float(TalentData.TalentPoints.PiercedTargetsOffset)/float(OwnerUnitBase->MaxTalentPoints));
			ProjectileScaler->SetPercent( float(TalentData.TalentPoints.ProjectileScaler)/float(OwnerUnitBase->MaxTalentPoints));
			ProjectileSpeedScaler->SetPercent( float(TalentData.TalentPoints.ProjectileSpeedScaler)/float(OwnerUnitBase->MaxTalentPoints));

			WeaponStunOffset->SetPercent( float(TalentData.TalentPoints.WeaponStunOffset)/float(OwnerUnitBase->MaxTalentPoints));
			LaserCountOffset->SetPercent( float(TalentData.TalentPoints.LaserCountOffset)/float(OwnerUnitBase->MaxTalentPoints));
			ShieldCountOffset->SetPercent( float(TalentData.TalentPoints.ShieldCountOffset)/float(OwnerUnitBase->MaxTalentPoints));

			WeaponName->SetText(FText::FromString(OwnerUnitBase->Weapons[OwnerUnitBase->ActualWeaponId]));
			AvailableTalents->SetText(FText::FromString(FString::FromInt(OwnerUnitBase->AvailableTalentPoints[OwnerUnitBase->ActualWeaponId])));

		}
	}
}

void UTalentChooser::AddRangeOffset()
{
	if (!OwnerUnitBase.IsValid())
		return;
	
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "RangeOffset");
}

void UTalentChooser::AddMagOffset()
{
	if (!OwnerUnitBase.IsValid())
		return;
	
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "MagOffset");
}

void UTalentChooser::AddAttackSpeedScaler()
{
		if (!OwnerUnitBase.IsValid())
    		return;
	
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "AttackSpeedScaler");
}

void UTalentChooser::AddPiercedTargetsOffset()
{
	if (!OwnerUnitBase.IsValid())
		return;
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "PiercedTargetsOffset");
}

void UTalentChooser::AddProjectileScaler()
{
	if (!OwnerUnitBase.IsValid())
		return;
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "ProjectileScaler");
}

void UTalentChooser::AddProjectileSpeedScaler()
{
	if (!OwnerUnitBase.IsValid())
		return;
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "ProjectileSpeedScaler");
}

void UTalentChooser::AddWeaponStunOffset()
{
	if (!OwnerUnitBase.IsValid())
		return;
	
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "WeaponStunOffset");
	
}

void UTalentChooser::AddLaserCountOffset()
{
	if (!OwnerUnitBase.IsValid())
		return;
	
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "LaserCountOffset");
	
}

void UTalentChooser::AddShieldCountOffset()
{
	if (!OwnerUnitBase.IsValid())
		return;
	
	OwnerUnitBase->IncrementTalentPoint(OwnerUnitBase->ActualWeaponId, "ShieldCountOffset");
}

void UTalentChooser::ResetWeapon()
{
	if (!OwnerUnitBase.IsValid())
		return;
	OwnerUnitBase->ResetTalentPoint(OwnerUnitBase->ActualWeaponId);
}

void UTalentChooser::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
	
	RangeOffsetButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddRangeOffset);
	MagOffsetButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddMagOffset);
	AttackSpeedScalerButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddAttackSpeedScaler);
	PiercedTargetsOffsetButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddPiercedTargetsOffset);
	ProjectileScalerButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddProjectileScaler);
	ProjectileSpeedScalerButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddProjectileSpeedScaler);
	WeaponStunOffsetButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddWeaponStunOffset);
	LaserCountOffsetButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddLaserCountOffset);
	ShieldCountOffsetButton ->OnClicked.AddUniqueDynamic(this, &UTalentChooser::AddShieldCountOffset);
	ResetButton->OnClicked.AddUniqueDynamic(this, &UTalentChooser::ResetWeapon);
}
