// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Core/UnitData.h"
#include "MouseBotBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API UMouseBotBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMouseBotBaseAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterStatus)
	TEnumAsByte<BotData::BotStatus> CharAnimState;

	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float Deltaseconds) override;
	
};
