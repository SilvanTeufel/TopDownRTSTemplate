// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Actors/Weapon/Projectile.h"
#include "Core/UnitData.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "WeaponBase.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		int MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		FString Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		UStaticMesh* ProjectileMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		UMaterialInterface* ProjectileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		FVector ProjectileScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float PauseDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float ThrowDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float ThrowRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		int MaxPiercedTargets;
};

UCLASS()
class TOPDOWNRTSTEMPLATE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TriggerCapsule", Keywords = "TopDownRTSTemplate TriggerCapsule"), Category = TopDownRTSTemplate)
	class UCapsuleComponent* TriggerCapsule;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponOverlap", Keywords = "TopDownRTSTemplate WeaponOverlap"), Category = TopDownRTSTemplate)
	void WeaponOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitState", Keywords = "TopDownRTSTemplate UnitState"), Category = TopDownRTSTemplate)
		TEnumAsByte<WeaponStates::WeaponState> WeaponState = WeaponStates::Normal;

	AExtendedUnitBase* UnitWhichThrowed;
		
	void SetWeaponFlying(AExtendedUnitBase* Target);
	void SetWeaponReturning(AExtendedUnitBase* Target);
	void WeaponIsFlying(AExtendedUnitBase* ExtendedUnitBase);
	void WeaponIsReturning(AExtendedUnitBase* ExtendedUnitBase);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetUnitState", Keywords = "TopDownRTSTemplate SetUnitState"), Category = TopDownRTSTemplate)
	void SetWeaponState( TEnumAsByte<WeaponStates::WeaponState> NewWeaponState)
	{
		WeaponState = NewWeaponState;
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "ThrowCollisionProfile", Keywords = "TopDownRTSTemplate ThrowCollisionProfile"), Category = TopDownRTSTemplate)
	FCollisionProfileName ThrowCollisionProfile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "NormalCollisionProfile", Keywords = "TopDownRTSTemplate NormalCollisionProfile"), Category = TopDownRTSTemplate)
	FCollisionProfileName NormalCollisionProfile;
		
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeWeapon", Keywords = "TopDownRTSTemplate ChangeWeapon"), Category = TopDownRTSTemplate)
	void ChangeWeapon(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponRange", Keywords = "TopDownRTSTemplate GetWeaponRange"), Category = TopDownRTSTemplate)
	float GetWeaponRange(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponMagSize", Keywords = "TopDownRTSTemplate GetWeaponMagSize"), Category = TopDownRTSTemplate)
	int GetWeaponMagSize(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponDamage", Keywords = "TopDownRTSTemplate GetWeaponDamage"), Category = TopDownRTSTemplate)
	float GetWeaponDamage(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponDamage", Keywords = "TopDownRTSTemplate GetWeaponDamage"), Category = TopDownRTSTemplate)
	float GetWeaponThrowDamage(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponRange", Keywords = "TopDownRTSTemplate GetWeaponRange"), Category = TopDownRTSTemplate)
	float GetWeaponThrowRange(FString WeaponName);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponTime", Keywords = "TopDownRTSTemplate GetWeaponTime"), Category = TopDownRTSTemplate)
	float GetWeaponTime(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponType", Keywords = "TopDownRTSTemplate GetWeaponType"), Category = TopDownRTSTemplate)
	FString GetWeaponType(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponMesh", Keywords = "TopDownRTSTemplate GetWeaponMesh"), Category = TopDownRTSTemplate)
	class USkeletalMesh* GetWeaponMesh(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponNames", Keywords = "TopDownRTSTemplate GetWeaponNames"), Category = TopDownRTSTemplate)
	TArray<FString> GetWeaponNames();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileMesh", Keywords = "TopDownRTSTemplate GetProjectileMesh"), Category = TopDownRTSTemplate)
	UStaticMesh* GetProjectileMesh(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileMaterial", Keywords = "TopDownRTSTemplate GetProjectileMaterial"), Category = TopDownRTSTemplate)
	UMaterialInterface* GetProjectileMaterial(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileSize", Keywords = "TopDownRTSTemplate GetProjectileSize"), Category = TopDownRTSTemplate)
	FVector GetProjectileScale(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetReloadTime", Keywords = "TopDownRTSTemplate GetReloadTime"), Category = TopDownRTSTemplate)
	float GetReloadTime(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileSpeed", Keywords = "TopDownRTSTemplate GetProjectileSpeed"), Category = TopDownRTSTemplate)
	float GetProjectileSpeed(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PauseDuration", Keywords = "TopDownRTSTemplate PauseDuration"), Category = TopDownRTSTemplate)
	float GetPauseDuration(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PauseDuration", Keywords = "TopDownRTSTemplate PauseDuration"), Category = TopDownRTSTemplate)
	int GetMaxPiercedTargets(FString WeaponName);
protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate) //BlueprintReadWrite, Category="Weapon-Specific"
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		float MaxWeaponNormalVelocity = 300.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PlayFireAnimation", Keywords = "TopDownRTSTemplate PlayFireAnimation"), Category = TopDownRTSTemplate)
	void PlayFireAnimation();

};
