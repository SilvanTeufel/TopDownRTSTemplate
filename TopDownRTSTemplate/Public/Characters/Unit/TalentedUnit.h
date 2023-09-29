// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Core/Talents.h"
#include "Core/TalentSaveGame.h"
#include "TalentedUnit.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API ATalentedUnit : public AExtendedUnitBase
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
	
public:
	ATalentedUnit(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class UDataTable* WeaponTalentTable;
	
	TArray<FWeaponTalents> WeaponTalentsArray;
	
	TArray<FWeaponTalentData> WeaponTalentDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool DisableSaveGame = false;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateWeponTalentData", Keywords = "RTSUnitTemplate CreateWeponTalentData"), Category = RTSUnitTemplate)
	void CreateWeaponTalentData();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IncrementTalentPoint", Keywords = "RTSUnitTemplate IncrementTalentPoint"), Category = RTSUnitTemplate)
	void IncrementTalentPoint(int32 WeaponId, const FString& TalentKey);

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void SetTalentPoints(int32 WeaponId);
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void ResetTalentPoint(int32 WeaponId);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int MaxTalentPoints = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int MaxAvailableTalentPoints = 25;
	
	TArray<int> AvailableTalentPoints;

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void AddTalentPoint(int Amount);
	
	UFUNCTION(Category = RTSUnitTemplate)
	void SaveTalentPoints(const TArray<FWeaponTalentData>& TalentDataArray);

	UFUNCTION(Category = RTSUnitTemplate)
	TArray<FWeaponTalentData> LoadTalentPoints();
	
	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate )
	float InitTalentWeaponStunOffset = TalentWeaponStunOffset;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate )
	int InitLaserCount = LaserCount;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate )
	float InitShieldMaxHitCounter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class UWidgetComponent* TimerWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector TimerWidgetCompLocation = FVector (0.f, 0.f, -180.f);

	UFUNCTION(Category = RTSUnitTemplate)
	void SetupTimerWidget();
};
