// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Controller/MouseBotControllerBase.h"
#include "GameFramework/PlayerController.h"

AMouseBotControllerBase::AMouseBotControllerBase()
{

}

void AMouseBotControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMouseBotControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MouseBotStateMachine(DeltaSeconds);
}

void AMouseBotControllerBase::MouseBotStateMachine(float DeltaTime)
{


	AMouseBotBase* MouseBot = Cast<AMouseBotBase>(GetPawn());


	MouseBot->LifeTime += DeltaTime;
	if(MouseBot->LifeTime > MouseBot->MaxLifeTime)
	{
		MouseBot->Destroy(true, false);
		Destroy(true, false);
	}



	const FVector CurrentLocation = MouseBot->GetActorLocation();
	
	switch (MouseBot->CharAnimState)
	{
	case BotData::Attack_Bot:
		{
			if(MouseBot->CurrentEnemy)
			{
				MouseBot->AttackIterationTime += DeltaTime;
				FVector EnemyOrigin;
				FVector EnemyBoxExtent;
				MouseBot->CurrentEnemy->GetActorBounds(false, EnemyOrigin, EnemyBoxExtent);
				FVector EDirection = UKismetMathLibrary::GetDirectionUnitVector(CurrentLocation, EnemyOrigin);

				if(CurrentLocation.Z < 50.0f)
					MouseBot->AttackBottom = false;
				else if(CurrentLocation.Z > EnemyBoxExtent.Z+50.f)
					MouseBot->AttackBottom = true;
				
				if(!MouseBot->AttackBottom)
				{
					MouseBot->LaunchCharacter(FVector(EDirection.X*100.f,EDirection.Y*100.f,150.f),true, true);
					if(MouseBot->AttackIterationTime >= 1.f)
					{
						MouseBot->AttackIterationTime = 0;
						MouseBot->CurrentEnemy->IsAttacked(this);
						MouseBot->CurrentEnemy->SetHealth(MouseBot->CurrentEnemy->GetHealth()-MouseBot->AttackDamage);
					}
				}else if(MouseBot->AttackBottom)
				{
					MouseBot->LaunchCharacter(FVector(EDirection.X*50.f,EDirection.Y*50.f,-50.f),true, true);
					if(MouseBot->AttackIterationTime >= 1.f)
					{
						MouseBot->AttackIterationTime = 0;
						MouseBot->CurrentEnemy->IsAttacked(this);
						MouseBot->CurrentEnemy->SetHealth(MouseBot->CurrentEnemy->GetHealth()-MouseBot->AttackDamage);
					}

					if(MouseBot->CurrentEnemy->GetUnitState() == UnitData::Dead)
					{
						MouseBot->SetAnimState(BotData::Run_Bot);
					}
				}
			}
			
		}
	break;
	case BotData::Heal_Bot:
		{
			if(MouseBot->CurrentFriend)
			{
				MouseBot->HealIterationTime += DeltaTime;
				FVector EnemyOrigin;
				FVector EnemyBoxExtent;
				MouseBot->CurrentFriend->GetActorBounds(false, EnemyOrigin, EnemyBoxExtent);
				FVector EDirection = UKismetMathLibrary::GetDirectionUnitVector(CurrentLocation, EnemyOrigin);

				if(CurrentLocation.Z < 50.0f)
					MouseBot->HealBottom = false;
				else if(CurrentLocation.Z > EnemyBoxExtent.Z+50.f)
					MouseBot->HealBottom = true;
	            
			
				if(!MouseBot->HealBottom)
				{
					MouseBot->LaunchCharacter(FVector(EDirection.X*100.f,EDirection.Y*100.f,150.f),true, true);
					if(MouseBot->HealIterationTime >= 1.f)
					{
						MouseBot->HealIterationTime = 0;
						
						MouseBot->CurrentFriend->SetHealth(MouseBot->CurrentFriend->GetHealth()+MouseBot->HealPoints);
						if(MouseBot->CurrentFriend->GetHealth() >= MouseBot->CurrentFriend->GetMaxHealth())
						{
							MouseBot->SetAnimState(BotData::Run_Bot);
						}
					}
				}else if(MouseBot->HealBottom)
				{
					MouseBot->LaunchCharacter(FVector(EDirection.X*50.f,EDirection.Y*50.f,-50.f),true, true);
					if(MouseBot->HealIterationTime >= 1.f)
					{
						MouseBot->HealIterationTime = 0;
						
						MouseBot->CurrentFriend->SetHealth(MouseBot->CurrentFriend->GetHealth()+MouseBot->HealPoints);
						if(MouseBot->CurrentFriend->GetHealth() >= MouseBot->CurrentFriend->GetMaxHealth())
						{
							MouseBot->SetAnimState(BotData::Run_Bot);
						}
					}

					if(MouseBot->CurrentFriend->GetUnitState() == UnitData::Dead)
					{
						MouseBot->SetAnimState(BotData::Run_Bot);
					}
				}
			}

			
		}
		break;		
	case BotData::Dead_Bot:
		{
			MouseBot->DeathTime += DeltaTime;
		}
	break;
	case BotData::Run_Bot:
		{
			const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(CurrentLocation, MouseBot->MoveToLocation);
			MouseBot->AddMovementInput(ADirection, 4);
		}
	break;
	default:
        	{

        	}
	break;	
	}
}

