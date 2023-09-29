// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/Unit/UnitBase.h"
#include "Hud/HUDBase.h"
#include "UnitControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AUnitControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AUnitControllerBase();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* Pawn) override;

	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UFUNCTION(BlueprintCallable,  Category = TopDownRTSTemplate)
		void OnUnitDetected(const TArray<AActor*>& DetectedUnits);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "bDisableUnitControllerBaseTick", Keywords = "RTSUnitTemplate bDisableUnitControllerBaseTick"), Category = RTSUnitTemplate)
		//bool bDisableUnitControllerBaseTick = false;
	
	virtual FRotator GetControlRotation() const override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "KillUnitBase", Keywords = "TopDownRTSTemplate KillUnitBase"), Category = TopDownRTSTemplate)
		void KillUnitBase(AUnitBase* UnitBase);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SightRadius", Keywords = "TopDownRTSTemplate SightRadius"), Category = TopDownRTSTemplate)
		float SightRadius = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SightAge", Keywords = "TopDownRTSTemplate SightAge"), Category = TopDownRTSTemplate)
		float SightAge = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LoseSightRadius", Keywords = "TopDownRTSTemplate LoseSightRadius"), Category = TopDownRTSTemplate)
		float LoseSightRadius = SightRadius + 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FieldOfView", Keywords = "TopDownRTSTemplate FieldOfView"), Category = TopDownRTSTemplate)
		float FieldOfView = 360.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DespawnTime", Keywords = "TopDownRTSTemplate DespawnTime"), Category = TopDownRTSTemplate)
		float DespawnTime = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsAttackedDuration", Keywords = "TopDownRTSTemplate IsAttackedDuration"), Category = TopDownRTSTemplate)
		float IsAttackedDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float IsStunnedDuration = 2.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackAngleTolerance", Keywords = "TopDownRTSTemplate AttackAngleTolerance"), Category = TopDownRTSTemplate)
		float AttackAngleTolerance = 0.f;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SightConfig", Keywords = "TopDownRTSTemplate SightConfig"), Category = TopDownRTSTemplate)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "DistanceToUnitToChase", Keywords = "TopDownRTSTemplate DistanceToUnitToChase"), Category = TopDownRTSTemplate)
		float DistanceToUnitToChase = 0.0f;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "DetectFriendlyUnits", Keywords = "RTSUnitTemplate DetectFriendlyUnits"), Category = RTSUnitTemplate)
		bool DetectFriendlyUnits = false;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateToAttackUnit", Keywords = "TopDownRTSTemplate RotateToAttackUnit"), Category = TopDownRTSTemplate)
		void RotateToAttackUnit(AUnitBase* AttackingUnit, AUnitBase* UnitToAttack);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnitControlStateMachine", Keywords = "TopDownRTSTemplate UnitControlStateMachine"), Category = TopDownRTSTemplate)
		void UnitControlStateMachine(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateProjectile", Keywords = "TopDownRTSTemplate CreateProjectile"), Category = TopDownRTSTemplate)
		void CreateProjectile (AUnitBase* UnitBase, AUnitBase* Target);
	
	bool ProjectileSpawned = false;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Dead", Keywords = "RTSUnitTemplate Dead"), Category = RTSUnitTemplate)
	bool IsUnitToChaseInRange(AUnitBase* UnitBase);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Dead", Keywords = "RTSUnitTemplate Dead"), Category = RTSUnitTemplate)
		void Dead(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Patrol", Keywords = "RTSUnitTemplate Patrol"), Category = RTSUnitTemplate)
		void Patrol(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Run", Keywords = "RTSUnitTemplate Run"), Category = RTSUnitTemplate)
		void Run(AUnitBase* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Chase", Keywords = "RTSUnitTemplate Chase"), Category = RTSUnitTemplate)
		void Chase(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Attack", Keywords = "RTSUnitTemplate Attack"), Category = RTSUnitTemplate)
		void Attack(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pause", Keywords = "RTSUnitTemplate Pause"), Category = RTSUnitTemplate)
		void Pause(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsAttacked", Keywords = "RTSUnitTemplate IsAttacked"), Category = RTSUnitTemplate)
		void IsAttacked(AUnitBase* UnitBase, float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
		void IsStunned(AUnitBase* UnitBase, float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Idle", Keywords = "RTSUnitTemplate Idle"), Category = RTSUnitTemplate)
		void Idle(AUnitBase* UnitBase, float DeltaSeconds);
	
};

