// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Widgets/UnitBaseHealthBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UUnitBaseHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerCharacter.IsValid())
		return;

	//OwnerCharacter->GetHealth();
	HealthBar->SetPercent(OwnerCharacter->GetHealth() / OwnerCharacter->GetMaxHealth());

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerCharacter->GetHealth(), &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(OwnerCharacter->GetMaxHealth(), &Opts));
}
