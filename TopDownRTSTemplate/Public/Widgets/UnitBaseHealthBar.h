// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Unit/UnitBase.h"
#include "UnitBaseHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API UUnitBaseHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwnerActor(AUnitBase* Enemy) {
		OwnerCharacter = Enemy;
	}


protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakObjectPtr<AUnitBase> OwnerCharacter;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHealthLabel;


};

