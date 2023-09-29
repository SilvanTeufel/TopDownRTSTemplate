// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Characters/MouseBotBase.h"

#include "Characters/Unit/UnitBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
// Sets default values
AMouseBotBase::AMouseBotBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// declare trigger capsule
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 55.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);


	// declare overlap events
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMouseBotBase::OnOverlapBegin); 
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AMouseBotBase::OnOverlapEnd); 

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->GravityScale = 1;
	SetAnimState(BotData::Idle_Bot);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorEnableCollision(true);
}

void AMouseBotBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap is Beginning") );
	if(GetAnimState() != BotData::Attack_Bot)
	{
		AUnitBase* UnitBase = nullptr;
		
		if(OtherActor)
		{
			UnitBase = Cast<AUnitBase>(OtherActor);
		}


		
		if(UnitBase)
		{
			if(!UnitBase->IsFriendly)
			{
		
				//UE_LOG(LogTemp, Warning, TEXT("SET ATTACK_BOT!") );
				CurrentEnemy = UnitBase;
				SetAnimState(BotData::Attack_Bot);
		
			}

			if(UnitBase->IsFriendly)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Heal this Character!") );
				CurrentFriend = UnitBase;
				SetAnimState(BotData::Heal_Bot);
			
			}
		}
	}
}

void AMouseBotBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

// Called when the game starts or when spawned
void AMouseBotBase::BeginPlay()
{
	Super::BeginPlay();
	SetAnimState(BotData::Idle_Bot);
	
}

// Called every frame
void AMouseBotBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector CurrentLocation = GetActorLocation();
	
	switch (CharAnimState)
	{
	case BotData::Attack_Bot:
		{
			if(CurrentEnemy)
			{
				AttackIterationTime += DeltaTime;
				FVector EnemyOrigin;
				FVector EnemyBoxExtent;
				CurrentEnemy->GetActorBounds(false, EnemyOrigin, EnemyBoxExtent);
				FVector EDirection = UKismetMathLibrary::GetDirectionUnitVector(CurrentLocation, EnemyOrigin);

				if(CurrentLocation.Z < 50.0f)
					AttackBottom = false;
				else if(CurrentLocation.Z > EnemyBoxExtent.Z+50.f)
					AttackBottom = true;
            
		
				if(!AttackBottom)
				{
					LaunchCharacter(FVector(EDirection.X*100.f,EDirection.Y*100.f,150.f),true, true);
					if(AttackIterationTime >= 1.f)
					{
						AttackIterationTime = 0;
						CurrentEnemy->IsAttacked(this);
						CurrentEnemy->SetHealth(CurrentEnemy->GetHealth()-AttackDamage);
					}
				}else if(AttackBottom)
				{
					LaunchCharacter(FVector(EDirection.X*50.f,EDirection.Y*50.f,-50.f),true, true);
					if(AttackIterationTime >= 1.f)
					{
						AttackIterationTime = 0;
						CurrentEnemy->IsAttacked(this);
						CurrentEnemy->SetHealth(CurrentEnemy->GetHealth()-AttackDamage);
					}

					if(CurrentEnemy->GetUnitState() == UnitData::Dead)
					{
						SetAnimState(BotData::Run_Bot);
					}
				}
			}
			
		}
	break;
	case BotData::Heal_Bot:
		{

			if(CurrentFriend)
			{
				HealIterationTime += DeltaTime;
				FVector EnemyOrigin;
				FVector EnemyBoxExtent;
				CurrentFriend->GetActorBounds(false, EnemyOrigin, EnemyBoxExtent);
				FVector EDirection = UKismetMathLibrary::GetDirectionUnitVector(CurrentLocation, EnemyOrigin);

				if(CurrentLocation.Z < 50.0f)
					HealBottom = false;
				else if(CurrentLocation.Z > EnemyBoxExtent.Z+50.f)
					HealBottom = true;
	            
			
				if(!HealBottom)
				{
					LaunchCharacter(FVector(EDirection.X*100.f,EDirection.Y*100.f,150.f),true, true);
					if(HealIterationTime >= 1.f)
					{
						HealIterationTime = 0;
						
						CurrentFriend->SetHealth(CurrentFriend->GetHealth()+HealPoints);
						if(CurrentFriend->GetHealth() >= CurrentFriend->GetMaxHealth())
						{
							SetAnimState(BotData::Run_Bot);
						}
					}
				}else if(HealBottom)
				{
					LaunchCharacter(FVector(EDirection.X*50.f,EDirection.Y*50.f,-50.f),true, true);
					if(HealIterationTime >= 1.f)
					{
						HealIterationTime = 0;
						
						CurrentFriend->SetHealth(CurrentFriend->GetHealth()+HealPoints);
						if(CurrentFriend->GetHealth() >= CurrentFriend->GetMaxHealth())
						{
							SetAnimState(BotData::Run_Bot);
						}
					}
					
					if(CurrentFriend->GetUnitState() == UnitData::Dead)
					{
						SetAnimState(BotData::Run_Bot);
					}
				}
			}
			
		}
		break;		
	case BotData::Dead_Bot:
		{
			DeathTime += DeltaTime;
		}
	break;
	case BotData::Run_Bot:
		{

			const FVector ADirection = UKismetMathLibrary::GetDirectionUnitVector(CurrentLocation, MoveToLocation);
			AddMovementInput(ADirection, 4);


		}
	break;	
	}
}

// Called to bind functionality to input
void AMouseBotBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMouseBotBase::KillBot()
{
	Destroy(true,true);
}

void AMouseBotBase::SetAnimState(TEnumAsByte<BotData::BotStatus> NewCharAnimState)
{
	CharAnimState = NewCharAnimState;
	
}

TEnumAsByte<BotData::BotStatus> AMouseBotBase::GetAnimState()
{
	return CharAnimState;
}