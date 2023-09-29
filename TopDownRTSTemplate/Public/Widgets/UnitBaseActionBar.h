// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Actors/Abilitys/Shield.h"
#include "UnitBaseActionBar.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API UUnitBaseActionBar : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
public:
	void SetOwnerActor(AExtendedUnitBase* Unit) {
		OwnerUnitBase = Unit;
		
		if(Unit->Shield)
			OwnerShield = Unit->Shield;
	}


protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
		TWeakObjectPtr<AExtendedUnitBase> OwnerUnitBase;

	UPROPERTY()
		TWeakObjectPtr<AShield> OwnerShield;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MagazineBar;
	
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ShieldBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ManaBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* EnergyBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableMagCountLabel;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableMagLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxMagLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableShieldLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxShieldLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableManaLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxManaLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableEnergyLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxEnergyLabel;
};
