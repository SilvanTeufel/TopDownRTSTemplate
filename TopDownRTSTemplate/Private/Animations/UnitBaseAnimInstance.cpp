// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Animations/UnitBaseAnimInstance.h"
#include "Characters/Unit/UnitBase.h"
#include "Core/UnitData.h"

UUnitBaseAnimInstance::UUnitBaseAnimInstance() {
	CharAnimState = UnitData::Idle;
}


void UUnitBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AUnitBase* Actor = Cast<AUnitBase>(OwningActor);
}

void UUnitBaseAnimInstance::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	OwningActor = GetOwningActor();
	
	if (OwningActor != nullptr) {
		AUnitBase* UnitBase = Cast<AUnitBase>(OwningActor);
		if (UnitBase != nullptr) {
			
			CharAnimState = UnitBase->GetUnitState();
			SetAnimTableData(CharAnimState);

			if(LastAnimState != CharAnimState)
			{
				SoundTimer = 0.f;
				LastAnimState = CharAnimState;
			}

			if(Sound && UnitBase)
			{
				if(SoundTimer == 0.f)
					UGameplayStatics::PlaySoundAtLocation(UnitBase, Sound, UnitBase->GetActorLocation(), 1.f);

				SoundTimer += Deltaseconds;
			}
			
			switch (CharAnimState)
			{
			case UnitData::Attack:
				{
					//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT(" Attack "));
					BlendPoint_1 -= (1/UnitBase->AttackDuration)*AttackTempoScale;
					BlendPoint_2 -= (1/UnitBase->AttackDuration)*AttackTempoScale;
				}
				break;
			case UnitData::IsAttacked:
				{
					//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT(" IsAttacked "));
					
				}
				break;	
			default:
				{
					//if(UnitBase->IsFriendly)UE_LOG(LogTemp, Warning, TEXT(" Other "));
				}
				break;
			}
			
			if(abs(CurrentBlendPoint_1-BlendPoint_1) <= Resolution_1) CurrentBlendPoint_1 = BlendPoint_1;
			else if(CurrentBlendPoint_1 < BlendPoint_1) CurrentBlendPoint_1 += TransitionRate_1;
			else if(CurrentBlendPoint_1 > BlendPoint_1) CurrentBlendPoint_1 += TransitionRate_1*(-1);

			if(abs(CurrentBlendPoint_2-BlendPoint_2) <= Resolution_2) CurrentBlendPoint_2 = BlendPoint_2;
			else if(CurrentBlendPoint_2 < BlendPoint_2) CurrentBlendPoint_2 += TransitionRate_2;
			else if(CurrentBlendPoint_2 > BlendPoint_2) CurrentBlendPoint_2 += TransitionRate_2*(-1);

			//if(Sound && UnitBase)
				//if(UnitBase->UnitControlTimer <= Deltaseconds)
				//UGameplayStatics::PlaySoundAtLocation(UnitBase, Sound, UnitBase->GetActorLocation(), 1.f);
				//UGameplayStatics::PlaySound2D(UnitBase, Sound, 0.5f);
		}
	}
}

void UUnitBaseAnimInstance::SetAnimTableData(TEnumAsByte<UnitData::EState> AnimState)
{
	if (AnimDataTable)
	{
		for(auto it : AnimDataTable->GetRowMap())
		{
			FString Key = it.Key.ToString();
			UnitAnimData = reinterpret_cast<FUnitAnimData*>(it.Value);
			if(UnitAnimData->AnimState == AnimState)
			{
				BlendPoint_1 = UnitAnimData->BlendPoint_1;
				BlendPoint_2 = UnitAnimData->BlendPoint_2;
				TransitionRate_1 = UnitAnimData->TransitionRate_1;
				TransitionRate_2 = UnitAnimData->TransitionRate_2;
				Resolution_1 = UnitAnimData->Resolution_1;
				Resolution_2 = UnitAnimData->Resolution_2;
				Sound = UnitAnimData->Sound;
			}
		}
		
		
	}

}
