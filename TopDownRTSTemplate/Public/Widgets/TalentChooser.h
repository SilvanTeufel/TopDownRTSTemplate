// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Talents.h"
#include "Characters/Unit/TalentedUnit.h"
#include "TalentChooser.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API UTalentChooser : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY()
	TWeakObjectPtr<ATalentedUnit> OwnerUnitBase;

	void CreateProgressBars();
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock*  WeaponName;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar*  RangeOffset;

	UPROPERTY(meta = (BindWidget))
	class UButton*  RangeOffsetButton;

	UFUNCTION()
	void AddRangeOffset();
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MagOffset;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  MagOffsetButton;

	UFUNCTION()
	void AddMagOffset();
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* AttackSpeedScaler;

	UPROPERTY(meta = (BindWidget))
	class UButton*  AttackSpeedScalerButton;

	UFUNCTION()
	void AddAttackSpeedScaler();
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PiercedTargetsOffset;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  PiercedTargetsOffsetButton;

	UFUNCTION()
	void AddPiercedTargetsOffset();
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProjectileScaler;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  ProjectileScalerButton;

	UFUNCTION()
	void AddProjectileScaler();
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProjectileSpeedScaler;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  ProjectileSpeedScalerButton;

	UFUNCTION()
	void AddProjectileSpeedScaler();

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* WeaponStunOffset;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  WeaponStunOffsetButton;

	UFUNCTION()
	void AddWeaponStunOffset();
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* LaserCountOffset;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  LaserCountOffsetButton;

	UFUNCTION()
	void AddLaserCountOffset();

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ShieldCountOffset;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  ShieldCountOffsetButton;

	UFUNCTION()
	void AddShieldCountOffset();
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock*  AvailableTalents;
	
	UPROPERTY(meta = (BindWidget))
	class UButton*  ResetButton;
	
	UFUNCTION()
	void ResetWeapon();
	
public:
	virtual void NativeConstruct() override;
	
	void SetOwnerActor(ATalentedUnit* Unit) {
		OwnerUnitBase = Unit;
	}
};
