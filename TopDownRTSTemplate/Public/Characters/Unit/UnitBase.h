// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Core/UnitData.h"
#include "Actors/IndicatorActor.h"
#include "SpawnerUnit.h"
#include "UnitBase.generated.h"


UCLASS()
class TOPDOWNRTSTEMPLATE_API AUnitBase : public ASpawnerUnit
{
	GENERATED_BODY()
public:
	AUnitBase(const FObjectInitializer& ObjectInitializer);

protected:
// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
// Called every frame
	virtual void Tick(float DeltaTime) override;

// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsFriendly", Keywords = "TopDownRTSTemplate IsFriendly"), Category = TopDownRTSTemplate)
		bool IsFriendly = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsFlying", Keywords = "TopDownRTSTemplate IsFlying"), Category = TopDownRTSTemplate)
		bool IsFlying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "FlyHeight", Keywords = "TopDownRTSTemplate FlyHeight"), Category = TopDownRTSTemplate)
		float FlyHeight = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Range", Keywords = "TopDownRTSTemplate Range"), Category = TopDownRTSTemplate)
		float Range = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StopChaseAtDistance", Keywords = "TopDownRTSTemplate StopChaseAtDistance"), Category = TopDownRTSTemplate)
		float StopChaseAtDistance = 100.f;
	
// RTSHud related //////////////////////////////////////////
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateCameraComp", Keywords = "TopDownRTSTemplate CreateCameraComp"), Category = TopDownRTSTemplate)
		void IsAttacked(AActor* AttackingCharacter); // AActor* SelectedCharacter

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
		float StunOffset = 0.f;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateCameraComp", Keywords = "TopDownRTSTemplate CreateCameraComp"), Category = TopDownRTSTemplate)
		void SetWalkSpeed(float Speed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxRunSpeed", Keywords = "TopDownRTSTemplate MaxRunSpeed"), Category = TopDownRTSTemplate)
		float MaxRunSpeed = 400.f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsAttackedSpeed", Keywords = "TopDownRTSTemplate IsAttackedSpeed"), Category = TopDownRTSTemplate)
		float IsAttackedSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "RunSpeedScale", Keywords = "TopDownRTSTemplate RunSpeedScale"), Category = TopDownRTSTemplate)
		float RunSpeedScale = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StopRunTolerance", Keywords = "TopDownRTSTemplate StopRunTolerance"), Category = TopDownRTSTemplate)
		float StopRunTolerance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StopRunToleranceY", Keywords = "TopDownRTSTemplate StopRunToleranceY"), Category = TopDownRTSTemplate)
		float StopRunToleranceY = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "StopRunToleranceY", Keywords = "RTSUnitTemplate StopRunToleranceY"), Category = RTSUnitTemplate)
		float StopRunToleranceForFlying = 100.f;
///////////////////////////////////////////////////////////////////

// related to Animations  //////////////////////////////////////////
public:

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "UnitControlTimer", Keywords = "TopDownRTSTemplate UnitControlTimer"), Category = TopDownRTSTemplate)
	float UnitControlTimer = 0.0f; // This Timer is used in UnitControllerBase Statemachine

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackDamage", Keywords = "TopDownRTSTemplate AttackDamage"), Category = TopDownRTSTemplate)
	float AttackDamage = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackDuration", Keywords = "TopDownRTSTemplate AttackDuration"), Category = TopDownRTSTemplate)
	float AttackDuration = 0.6f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "PauseDuration", Keywords = "TopDownRTSTemplate PauseDuration"), Category = TopDownRTSTemplate)
	float PauseDuration = 0.6f; // Duratin of the State Pause
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool ToggleUnitDetection = true;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "UnitToChase", Keywords = "TopDownRTSTemplate UnitToChase"), Category = TopDownRTSTemplate)
	AUnitBase* UnitToChase;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "UnitsToChase", Keywords = "TopDownRTSTemplate UnitsToChase"), Category = TopDownRTSTemplate)
	TArray <AUnitBase*> UnitsToChase;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetNextUnitToChase", Keywords = "TopDownRTSTemplate SetNextUnitToChase"), Category = TopDownRTSTemplate)
	bool SetNextUnitToChase();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "DistanceToUnitToChase", Keywords = "TopDownRTSTemplate DistanceToUnitToChase"), Category = TopDownRTSTemplate)
	float DistanceToUnitToChase;
///////////////////////////////////////////////////////

// related to Waypoints  //////////////////////////////////////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "NextWaypoint", Keywords = "TopDownRTSTemplate NextWaypoint"), Category = TopDownRTSTemplate)
	class AWaypoint* NextWaypoint;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetWaypoint", Keywords = "TopDownRTSTemplate SetWaypoint"), Category = TopDownRTSTemplate)
	void SetWaypoint(class AWaypoint* NewNextWaypoint);
///////////////////////////////////////////////////////////////////

// Set Unit States  //////////////////////////////////////////
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetUnitState", Keywords = "TopDownRTSTemplate SetUnitState"), Category = TopDownRTSTemplate)
	void SetUnitState( TEnumAsByte<UnitData::EState> NewUnitState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetUnitState", Keywords = "TopDownRTSTemplate GetUnitState"), Category = TopDownRTSTemplate)
	TEnumAsByte<UnitData::EState> GetUnitState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitState", Keywords = "TopDownRTSTemplate UnitState"), Category = TopDownRTSTemplate)
	TEnumAsByte<UnitData::EState> UnitState = UnitData::Idle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitStatePlaceholder", Keywords = "TopDownRTSTemplate UnitStatePlaceholder"), Category = TopDownRTSTemplate)
	TEnumAsByte<UnitData::EState> UnitStatePlaceholder = UnitData::Patrol;
///////////////////////////////////////////////////////////////////

// related to Healthbar  //////////////////////////////////////////
public:
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetupHealthWidget();
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMaxHealth", Keywords = "TopDownRTSTemplate GetMaxHealth"), Category = TopDownRTSTemplate)
	float GetMaxHealth();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealthWidgetComp", Keywords = "TopDownRTSTemplate HealthWidgetComp"), Category = TopDownRTSTemplate)
		class UWidgetComponent* HealthWidgetComp;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Health", Keywords = "TopDownRTSTemplate Health"), Category = TopDownRTSTemplate)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxHealth", Keywords = "TopDownRTSTemplate MaxHealth"), Category = TopDownRTSTemplate)
	float MaxHealth = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealthWidgetCompLocation", Keywords = "TopDownRTSTemplate HealthWidgetCompLocation"), Category = TopDownRTSTemplate)
	FVector HealthWidgetCompLocation = FVector (0.f, 0.f, 180.f);
///////////////////////////////////////////////////////////////////


// HUDBase related ///////////
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetSelected", Keywords = "TopDownRTSTemplate SetSelected"), Category = TopDownRTSTemplate)
	void SetSelected();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetDeselected", Keywords = "TopDownRTSTemplate SetDeselected"), Category = TopDownRTSTemplate)
	void SetDeselected();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "RunLocationArray", Keywords = "TopDownRTSTemplate RunLocationArray"), Category = TopDownRTSTemplate)
	TArray <FVector> RunLocationArray;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "RunLocationArrayIterator", Keywords = "TopDownRTSTemplate RunLocationArrayIterator"), Category = TopDownRTSTemplate)
	int32 RunLocationArrayIterator;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector RunLocation;
/////////////////////////////

// SelectedIcon related /////////
protected:
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedCharacterIcon", Keywords = "TopDownRTSTemplate SelectedCharacterIcon"), Category = TopDownRTSTemplate)
	class ASelectedIcon* SelectedIcon;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnSelectedIcon", Keywords = "TopDownRTSTemplate SpawnSelectedIcon"), Category = TopDownRTSTemplate)
	void SpawnSelectedIcon();
//////////////////////////////////////


// Projectile related /////////
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileBaseClass", Keywords = "TopDownRTSTemplate ProjectileBaseClass"), Category = TopDownRTSTemplate)
	TSubclassOf<class AProjectile> ProjectileBaseClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Projectile", Keywords = "TopDownRTSTemplate Projectile"), Category = TopDownRTSTemplate)
	class AProjectile* Projectile;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnProjectile", Keywords = "TopDownRTSTemplate SpawnProjectile"), Category = TopDownRTSTemplate)
	void SpawnProjectile(AActor* Target, AActor* Attacker);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UseProjectile", Keywords = "TopDownRTSTemplate UseProjectile"), Category = TopDownRTSTemplate)
	bool UseProjectile = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileSpawnOffset", Keywords = "TopDownRTSTemplate ProjectileSpawnOffset"), Category = TopDownRTSTemplate)
	FVector ProjectileSpawnOffset = FVector(0.f,0.f,0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileScaleActorDirectionOffset", Keywords = "TopDownRTSTemplate ProjectileScaleActorDirectionOffset"), Category = TopDownRTSTemplate)
	float ProjectileScaleActorDirectionOffset = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileSpeed", Keywords = "TopDownRTSTemplate ProjectileSpeed"), Category = TopDownRTSTemplate)
	float ProjectileSpeed = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ProjectileScale", Keywords = "TopDownRTSTemplate ProjectileScale"), Category = TopDownRTSTemplate)
	FVector ProjectileScale = FVector(1.f,1.f,1.f);

	UPROPERTY(EditAnywhere, Category = TopDownRTSTemplate)
	TSubclassOf<class AIndicatorActor> IndicatorBaseClass;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SpawnIndicator(float Damage, FVector Location);
//////////////////////////////////////
	

// Used for Despawn  //////////////////////////////////////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DestroyAfterDeath", Keywords = "TopDownRTSTemplate DestroyAfterDeath"), Category = TopDownRTSTemplate)
	bool DestroyAfterDeath = true;
///////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FString Name = "Unit";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsHiddenAtStart = false;
	
};






