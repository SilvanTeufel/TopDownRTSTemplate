// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Widgets/UnitBaseActionBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>


FString IntToFStringWithLeadingZeroes(int32 Number)
{
	FString NumberString = FString::Printf(TEXT("%03d"), Number);
	return NumberString;
}

void UUnitBaseActionBar::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Hidden);
}

void UUnitBaseActionBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerUnitBase.IsValid() || !OwnerShield.IsValid())
		return;

	OwnerUnitBase->GetHealth();

	if (!MagazineBar)
		return;
	
	MagazineBar->SetPercent(float(OwnerUnitBase->MagSize[OwnerUnitBase->ActualWeaponId]) / float(OwnerUnitBase->MaxMagSize[OwnerUnitBase->ActualWeaponId]));

	if (!ShieldBar)
		return;
	
	ShieldBar->SetPercent( float(OwnerShield->GetHitCounter()) /float(OwnerShield->GetMaxHitCounter()) );

	if (!ManaBar)
		return;
	
	ManaBar->SetPercent( OwnerUnitBase->Mana / OwnerUnitBase->MaxMana );

	if (!EnergyBar)
		return;

	EnergyBar->SetPercent( OwnerUnitBase->Energy / OwnerUnitBase->MaxEnergy );
	
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	if (!AvailableMagLabel || !AvailableMagCountLabel || !MaxMagLabel || !AvailableShieldLabel || !MaxShieldLabel || !AvailableManaLabel || !MaxManaLabel || !AvailableManaLabel || !MaxEnergyLabel)
		return;

	//SetVisibility(ESlateVisibility::Visible);
	// Update the label text with leading zeroes
	AvailableMagCountLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerUnitBase->MagCount[OwnerUnitBase->ActualWeaponId])));
	AvailableMagLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerUnitBase->MagSize[OwnerUnitBase->ActualWeaponId])));
	MaxMagLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerUnitBase->MaxMagSize[OwnerUnitBase->ActualWeaponId])));

	AvailableShieldLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerShield->GetHitCounter())));
	MaxShieldLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerShield->GetMaxHitCounter())));

	AvailableManaLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerUnitBase->Mana)));
	MaxManaLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerUnitBase->MaxMana)));

	AvailableEnergyLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerUnitBase->Energy)));
	MaxEnergyLabel->SetText(FText::FromString(IntToFStringWithLeadingZeroes(OwnerUnitBase->MaxEnergy)));

}
