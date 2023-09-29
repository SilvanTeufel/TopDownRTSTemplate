// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/UnitBase.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Core/UnitData.h"
#include "MouseBotBase.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AMouseBotBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMouseBotBase();

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;
	
	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void KillBot();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "setAnimStateMouseBot", Keywords = "TopDownRTSTemplate setAnimStateEnemy"), Category = TopDownRTSTemplate)
	void SetAnimState(TEnumAsByte<BotData::BotStatus> NewCharAnimState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "getAnimStateMouseBot", Keywords = "TopDownRTSTemplate getAnimStateEnemy"), Category = TopDownRTSTemplate)
	TEnumAsByte<BotData::BotStatus> GetAnimState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CharAnimStateMouseBot", Keywords = "TopDownRTSTemplate CharAnimStateEnemy"), Category = TopDownRTSTemplate)
	TEnumAsByte<BotData::BotStatus> CharAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MoveToLocation", Keywords = "TopDownRTSTemplate CharAnimStateEnemy"), Category = TopDownRTSTemplate)
	FVector MoveToLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CurrentEnemy", Keywords = "TopDownRTSTemplate CurrentEnemy"), Category = TopDownRTSTemplate)
	AUnitBase* CurrentEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackBottom", Keywords = "TopDownRTSTemplate AttackBottom"), Category = TopDownRTSTemplate)
	bool AttackBottom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackIterationTime", Keywords = "TopDownRTSTemplate AttackIterationTime"), Category = TopDownRTSTemplate)
	float AttackIterationTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackDamage", Keywords = "TopDownRTSTemplate AttackDamage"), Category = TopDownRTSTemplate)
	float AttackDamage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DeathTime", Keywords = "TopDownRTSTemplate DeathTime"), Category = TopDownRTSTemplate)
	float DeathTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CurrentFriend", Keywords = "TopDownRTSTemplate CurrentFriend"), Category = TopDownRTSTemplate)
	AUnitBase* CurrentFriend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealBottom", Keywords = "TopDownRTSTemplate HealBottom"), Category = TopDownRTSTemplate)
	bool HealBottom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealIterationTime", Keywords = "TopDownRTSTemplate HealIterationTime"), Category = TopDownRTSTemplate)
	float HealIterationTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealPoints", Keywords = "TopDownRTSTemplate HealPoints"), Category = TopDownRTSTemplate)
	float HealPoints = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LifeTime", Keywords = "TopDownRTSTemplate LifeTime"), Category = TopDownRTSTemplate)
	float LifeTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxLifeTime", Keywords = "TopDownRTSTemplate MaxLifeTime"), Category = TopDownRTSTemplate)
	float MaxLifeTime = 15;
	
};
