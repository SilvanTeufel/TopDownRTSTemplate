// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/Talents.h"
#include "TalentSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API UTalentSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TArray<FWeaponTalentData> WeaponTalentDataArray;

	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	TArray<int> AvailableTalentPoints;
};
