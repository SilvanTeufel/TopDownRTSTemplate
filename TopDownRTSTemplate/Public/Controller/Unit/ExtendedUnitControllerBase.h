// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HoldPositionController.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Controller/Unit/UnitControllerBase.h"
#include "Hud/HUDBase.h"
#include "ExtendedUnitControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AExtendedUnitControllerBase : public AHoldPositionController
{
	GENERATED_BODY()
	
public:
	AExtendedUnitControllerBase();
	
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ExtendedUnitControlMachine", Keywords = "TopDownRTSTemplate ExtendedUnitControlMachine"), Category = TopDownRTSTemplate)
	void ExtendedUnitControlMachine(float DeltaSeconds);

	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void Charge(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void Jump(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void NoMana(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void PrepareMouseBot(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void PrepareScatterMine(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void TeleportEnd(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void TeleportStart(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetBackToIdleAndCreateRadius(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void Reload(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void UseGravity(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void WeaponGravity(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void ThrowWeapon(AExtendedUnitBase* ExtendedUnit, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void ExtendedAttack(AExtendedUnitBase* ExtendedUnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void ExtendedChase(AExtendedUnitBase* ExtendedUnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void ExtendedPause(AExtendedUnitBase* ExtendedUnitBase, float DeltaSeconds);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FRotator GravityRotation = FRotator( 1.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float WeaponGravityTime = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float WeaponThrowTime = 0.4f;
};
