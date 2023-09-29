// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HoldPositionUnit.h"
#include "Characters/Unit/LaserUnit.h"
#include "Characters/MouseBotBase.h"
#include "Actors/Scatter/ScatterMine.h"
#include "Actors/Abilitys/Shield.h"
#include "ExtendedUnitBase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AExtendedUnitBase : public ALaserUnit
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Weaponbase related //////////////
protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "WeaponClass", Keywords = "TopDownRTSTemplate WeaponClass"), Category = TopDownRTSTemplate)
	TSubclassOf<class AWeaponBase> WeaponClass;
	
public:
	AExtendedUnitBase(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(Visibleanywhere, BlueprintReadWrite, meta = (DisplayName = "Weapon", Keywords = "TopDownRTSTemplate Weapon"), Category = TopDownRTSTemplate)
		class AWeaponBase* Weapon;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponSpawn", Keywords = "TopDownRTSTemplate WeaponSpawn"), Category = TopDownRTSTemplate)
		void WeaponSpawn();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponLock", Keywords = "TopDownRTSTemplate WeaponLock"), Category = TopDownRTSTemplate)
		void WeaponLock();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponUnLock", Keywords = "TopDownRTSTemplate WeaponUnLock"), Category = TopDownRTSTemplate)
		void WeaponUnLock();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "WeaponUnlocked", Keywords = "TopDownRTSTemplate WeaponUnlocked"), Category = TopDownRTSTemplate)
		bool WeaponUnlocked = false;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeWeapon", Keywords = "TopDownRTSTemplate ChangeWeapon"), Category = TopDownRTSTemplate)
		void ChangeWeapon(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire", Keywords = "TopDownRTSTemplate Fire"), Category = TopDownRTSTemplate)
		void Fire();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponMesh", Keywords = "TopDownRTSTemplate GetWeaponMesh"), Category = TopDownRTSTemplate)
		class USkeletalMesh* GetWeaponMesh();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "WeaponArray", Keywords = "TopDownRTSTemplate WeaponArray"), Category = TopDownRTSTemplate)
		TArray<FString> Weapons;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MaxMagSize", Keywords = "TopDownRTSTemplate MaxMagSize"), Category = TopDownRTSTemplate)
		TArray<int> MaxMagSize;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MagSize", Keywords = "TopDownRTSTemplate MagSize"), Category = TopDownRTSTemplate)
		TArray<int> MagSize;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MagCount", Keywords = "TopDownRTSTemplate MagCount"), Category = TopDownRTSTemplate)
		TArray<int> MagCount;

	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetAmmo", Keywords = "TopDownRTSTemplate SetAmmo"), Category = TopDownRTSTemplate)
		void AddMagCount(int WeaponId, int Amount);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetAmmo", Keywords = "TopDownRTSTemplate SetAmmo"), Category = TopDownRTSTemplate)
		void SetAmmo(int NewAmmo);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAmmo", Keywords = "TopDownRTSTemplate GetAmmo"), Category = TopDownRTSTemplate)
		float GetAmmo();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ReloadWeapon", Keywords = "TopDownRTSTemplate ReloadWeapon"), Category = TopDownRTSTemplate)
		void ReloadWeapon();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ActualWeaponId", Keywords = "TopDownRTSTemplate ActualWeaponId"), Category = TopDownRTSTemplate)
		int ActualWeaponId = 1;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ProjectileMaterial", Keywords = "TopDownRTSTemplate ProjectileMaterial"), Category = TopDownRTSTemplate)
		UMaterialInterface* ProjectileMaterial;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ProjectileMesh", Keywords = "TopDownRTSTemplate ProjectileMesh"), Category = TopDownRTSTemplate)
		UStaticMesh* ProjectileMesh;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ReloadTime", Keywords = "TopDownRTSTemplate ReloadTime"), Category = TopDownRTSTemplate)
	   float ReloadTime;
	//////////////////////////////////////


	// Teleport related /////////
	UPROPERTY(Editanywhere, meta = (DisplayName = "TeleportStartTime", Keywords = "TopDownRTSTemplate TeleportStartTime"), Category = TopDownRTSTemplate)
	float TeleportStartTime = 1.f;

	UPROPERTY(Editanywhere, meta = (DisplayName = "TeleportEndTime", Keywords = "TopDownRTSTemplate TeleportEndTime"), Category = TopDownRTSTemplate)
	float TeleportEndTime = 0.5f;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "TeleportRadius", Keywords = "TopDownRTSTemplate TeleportRadius"), Category = TopDownRTSTemplate)
	float TeleportRadius = 70.f;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "TeleportFailedDmg", Keywords = "TopDownRTSTemplate TeleportFailedDmg"), Category = TopDownRTSTemplate)
	float TeleportFailedDmg = 10.f; // When TeleportStartTime reaches TeleportStopEndTime States is changes to Idle

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "TeleportLocation", Keywords = "TopDownRTSTemplate TeleportLocation"), Category = TopDownRTSTemplate)
	FVector TeleportLocation;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "TeleportManaCost", Keywords = "TopDownRTSTemplate TeleportManaCost"), Category = TopDownRTSTemplate)
	float TeleportManaCost = 30.f;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateTeleportRadius", Keywords = "TopDownRTSTemplate CreateTeleportRadius"), Category = TopDownRTSTemplate)
	void CreateTeleportRadius();

	UPROPERTY(Editanywhere, Category = TopDownRTSTemplate )
	float TalentWeaponStunOffset = 0.f;
	
	//////////////////////////////////////


	// ScatterMine related /////////
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScatterMineBaseClass", Keywords = "TopDownRTSTemplate ScatterMineBaseClass"), Category = TopDownRTSTemplate)
	TSubclassOf<class AScatterMine> ScatterMineBaseClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ScatterMine", Keywords = "TopDownRTSTemplate ScatterMine"), Category = TopDownRTSTemplate)
	class AScatterMine*  ScatterMine;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnScatterMine", Keywords = "TopDownRTSTemplate SpawnScatterMine"), Category = TopDownRTSTemplate)
	void SpawnScatterMine(AActor* Target, AActor* Attacker);

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ScatterMineSpawnLocation", Keywords = "TopDownRTSTemplate ScatterMineSpawnLocation"), Category = TopDownRTSTemplate)
	FVector ScatterMineSpawnLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScatterMineCastTime", Keywords = "TopDownRTSTemplate ScatterMineCastTime"), Category = TopDownRTSTemplate)
	float ScatterMineCastTime = 0.4f;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateMineSpawnRadius", Keywords = "TopDownRTSTemplate CreateMineSpawnRadius"), Category = TopDownRTSTemplate)
	void CreateMineSpawnRadius();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MineSpawnRadius", Keywords = "TopDownRTSTemplate MineSpawnRadius"), Category = TopDownRTSTemplate)
	float MineSpawnRadius = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MineManaCost", Keywords = "TopDownRTSTemplate MineManaCost"), Category = TopDownRTSTemplate)
	float MineManaCost = 15.f;
	//////////////////////////////////////


	
	// SelectedIcon related /////////
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIconActionRadius", Keywords = "TopDownRTSTemplate CreateIconActionRadius"), Category = TopDownRTSTemplate)
	void CreateIconActionRadius();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIconThrowRangeRadius", Keywords = "TopDownRTSTemplate CreateIconThrowRangeRadius"), Category = TopDownRTSTemplate)
	void CreateIconThrowRangeRadius();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIconGravityRangeRadius", Keywords = "TopDownRTSTemplate CreateIconGravityRangeRadius"), Category = TopDownRTSTemplate)
	void CreateIconGravityRangeRadius();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIconNormalRadius", Keywords = "TopDownRTSTemplate CreateIconNormalRadius"), Category = TopDownRTSTemplate)
	void CreateIconNormalRadius();
	//////////////////////////////////////

	
	/// Spellshield related /////////
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ShieldBaseClass", Keywords = "TopDownRTSTemplate ShieldBaseClass"), Category = TopDownRTSTemplate)
	TSubclassOf<class AShield> ShieldBaseClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Shield", Keywords = "TopDownRTSTemplate Shield"), Category = TopDownRTSTemplate)
	class AShield* Shield;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ToggleShield", Keywords = "TopDownRTSTemplate ToggleShield"), Category = TopDownRTSTemplate)
	void ToggleShield();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnShield", Keywords = "TopDownRTSTemplate SpawnShield"), Category = TopDownRTSTemplate)
	void SpawnShield();
	//////////////////////////////////////

	
	/// Mousebot related /////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MouseBotBaseClass", Keywords = "TopDownRTSTemplate MouseBotBaseClass"), Category = TopDownRTSTemplate)
	TSubclassOf<class AMouseBotBase> MouseBotBaseClass;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MouseBotSpawnLocation", Keywords = "TopDownRTSTemplate MouseBotSpawnLocation"), Category = TopDownRTSTemplate)
	FVector MouseBotSpawnLocation;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MouseBot", Keywords = "TopDownRTSTemplate MouseBot"), Category = TopDownRTSTemplate)
	class AMouseBotBase* MouseBot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MouseBotCastTime", Keywords = "TopDownRTSTemplate MouseBotCastTime"), Category = TopDownRTSTemplate)
	float MouseBotCastTime = 0.4f;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateMineSpawnRadius", Keywords = "TopDownRTSTemplate CreateMineSpawnRadius"), Category = TopDownRTSTemplate)
	void CreateMouseBotSpawnRadius();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MouseBotRadius", Keywords = "TopDownRTSTemplate MouseBotRadius"), Category = TopDownRTSTemplate)
	float MouseBotRadius = 15.f;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "BotManaCost", Keywords = "TopDownRTSTemplate BotManaCost"), Category = TopDownRTSTemplate)
	float BotManaCost = 5.f;
    	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnMouseBot", Keywords = "TopDownRTSTemplate SpawnMouseBot"), Category = TopDownRTSTemplate)
	void SpawnMouseBot();
	///////////////////////////////////////

	
	/// Mana related /////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxMana", Keywords = "TopDownRTSTemplate MaxMana"), Category = TopDownRTSTemplate)
	float MaxMana = 100.f;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Mana", Keywords = "TopDownRTSTemplate Mana"), Category = TopDownRTSTemplate)
	float Mana = MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ManaRestore", Keywords = "TopDownRTSTemplate ManaRestore"), Category = TopDownRTSTemplate)
	float ManaRestore = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ManaRestoreTime", Keywords = "TopDownRTSTemplate ManaRestoreTime"), Category = TopDownRTSTemplate)
	float ManaRestoreTime = 1.f;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ManaTime", Keywords = "TopDownRTSTemplate ManaTime"), Category = TopDownRTSTemplate)
	float ManaTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "NoManaTime", Keywords = "TopDownRTSTemplate NoManaTime"), Category = TopDownRTSTemplate)
	float NoManaTime = 1.5f;
	
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RestoreMana", Keywords = "TopDownRTSTemplate RestoreMana"), Category = TopDownRTSTemplate)
	void RestoreMana(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetMana", Keywords = "TopDownRTSTemplate SetMana"), Category = TopDownRTSTemplate)
	void SetMana(float NewMana);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMana", Keywords = "TopDownRTSTemplate GetMana"), Category = TopDownRTSTemplate)
	float GetMana();
	///////////////////////////////////////

	/// Jump related /////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpTime", Keywords = "TopDownRTSTemplate JumpTime"), Category = TopDownRTSTemplate)
	float JumpTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpPauseTime", Keywords = "TopDownRTSTemplate JumpPauseTime"), Category = TopDownRTSTemplate)
	float JumpPauseTime = 0.1f;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "JumpLocation", Keywords = "TopDownRTSTemplate JumpLocation"), Category = TopDownRTSTemplate)
	FVector JumpLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpLoad", Keywords = "TopDownRTSTemplate JumpLoad"), Category = TopDownRTSTemplate)
	float JumpLoad = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxJumpLoad", Keywords = "TopDownRTSTemplate MaxJumpLoad"), Category = TopDownRTSTemplate)
	float MaxJumpLoad = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpLoadOffset", Keywords = "TopDownRTSTemplate JumpLoadOffset"), Category = TopDownRTSTemplate)
	float JumpLoadOffset = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpCounter", Keywords = "TopDownRTSTemplate JumpCounter"), Category = TopDownRTSTemplate)
	int JumpCounter = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpCounterResetTime", Keywords = "TopDownRTSTemplate JumpCounterResetTime"), Category = TopDownRTSTemplate)
	float JumpCounterResetTime = 4.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpCounterResetTime", Keywords = "TopDownRTSTemplate JumpCounterResetTime"), Category = TopDownRTSTemplate)
	float JumpCounterTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxJumps", Keywords = "TopDownRTSTemplate MaxJumps"), Category = TopDownRTSTemplate)
	int MaxJumps = 2.f;
	///////////////////////////////////////

	/// Gravity related /////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GravityVector", Keywords = "TopDownRTSTemplate GravityVector"), Category = TopDownRTSTemplate)
	FVector MouseLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GravityIntervall", Keywords = "TopDownRTSTemplate GravityIntervall"), Category = TopDownRTSTemplate)
	float GravityIntervall = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GravityActor", Keywords = "TopDownRTSTemplate GravityActor"), Category = TopDownRTSTemplate)
	AGravityActor* GravityActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GravityScaler", Keywords = "TopDownRTSTemplate GravityScaler"), Category = TopDownRTSTemplate)
	float GravityScaler = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GravityScaler", Keywords = "TopDownRTSTemplate GravityScaler"), Category = TopDownRTSTemplate)
	float GravityRange = 1500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GravityScaler", Keywords = "TopDownRTSTemplate GravityScaler"), Category = TopDownRTSTemplate)
	float ThrowScaler = 35.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitToThrowWeapon", Keywords = "TopDownRTSTemplate UnitToThrowWeapon"), Category = TopDownRTSTemplate)
	AUnitBase* UnitToThrowWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitDidThrowWeapon", Keywords = "TopDownRTSTemplate UnitDidThrowWeapon"), Category = TopDownRTSTemplate)
	bool UnitDidThrowWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ThrowRange", Keywords = "TopDownRTSTemplate ThrowRange"), Category = TopDownRTSTemplate)
	float ThrowRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ThrowRange", Keywords = "TopDownRTSTemplate ThrowRange"), Category = TopDownRTSTemplate)
	int MaxPiercedTargets = 1;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetWeaponFlying", Keywords = "TopDownRTSTemplate SetWeaponFlying"), Category = TopDownRTSTemplate)
	void SetWeaponFlying();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetReturning", Keywords = "TopDownRTSTemplate SetReturning"), Category = TopDownRTSTemplate)
	void SetWeaponReturning();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ShowThrowRadius", Keywords = "TopDownRTSTemplate ShowThrowRadius"), Category = TopDownRTSTemplate)
	bool ShowThrowRadius = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ShowGravityRadius", Keywords = "TopDownRTSTemplate ShowGravityRadius"), Category = TopDownRTSTemplate)
	bool ShowGravityRadius = false;
	///////////////////////////////////////
	
	/// ChargeTime related /////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ChargeTime", Keywords = "TopDownRTSTemplate ChargeTime"), Category = TopDownRTSTemplate)
	float ChargeTime = 0.5f;
	///////////////////////////////////////
};

