// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Animations/MouseBotBaseAnimInstance.h"
#include "Characters/MouseBotBase.h"

UMouseBotBaseAnimInstance::UMouseBotBaseAnimInstance() {
	CharAnimState = BotData::Idle_Bot;
}


void UMouseBotBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UMouseBotBaseAnimInstance::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	AActor* OwningActor = GetOwningActor();

	if (OwningActor != nullptr) {
		AMouseBotBase* Actor = Cast<AMouseBotBase>(OwningActor);
		if (Actor != nullptr) {
			CharAnimState = Actor->GetAnimState();
		}
	}
}
