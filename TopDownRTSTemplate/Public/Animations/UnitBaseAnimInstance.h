// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Core/UnitData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Sound\SoundCue.h"
#include "UnitBaseAnimInstance.generated.h"


USTRUCT(BlueprintType)
struct FUnitAnimData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TEnumAsByte<UnitData::EState> AnimState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float BlendPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float BlendPoint_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float TransitionRate_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float TransitionRate_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Resolution_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Resolution_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	USoundBase* Sound;
};


/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class TOPDOWNRTSTEMPLATE_API UUnitBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UUnitBaseAnimInstance();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	AActor* OwningActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		TEnumAsByte<UnitData::EState> CharAnimState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float BlendPoint_1 = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float BlendPoint_2 = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float CurrentBlendPoint_1 = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float CurrentBlendPoint_2 = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float TransitionRate_1 = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float TransitionRate_2 = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Resolution_1 = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float Resolution_2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float AttackTempoScale = 1.75;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float SoundTimer = 0.f;
	
	TEnumAsByte<UnitData::EState> LastAnimState = UnitData::None;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
		class UDataTable* AnimDataTable;

	void SetAnimTableData(TEnumAsByte<UnitData::EState> AnimState);

	FUnitAnimData* UnitAnimData;
	
};