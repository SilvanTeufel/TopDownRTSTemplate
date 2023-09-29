 // Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Controller/ControllerBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Characters/Unit/LaserUnit.h"
#include "Widgets/UnitWidgetSelector.h"
#include "Characters/Unit/TalentedUnit.h"
#include "GameFramework/CharacterMovementComponent.h"


AControllerBase::AControllerBase() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}


void AControllerBase::BeginPlay() {
		HUDBase = Cast<APathProviderHUD>(GetHUD());
		ExtendedCameraBase = Cast<AExtendedCameraBase>(GetPawn());
		
}


void AControllerBase::SetupInputComponent() {

	Super::SetupInputComponent();

}




void AControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(HUDBase && ExtendedCameraBase)
	if(HUDBase->CreatedGridAndDijkstra || HUDBase->StopLoading)
	{
		ExtendedCameraBase->BlockControls = false;
		ExtendedCameraBase->DeSpawnLoadingWidget();
	}

	
	SelectedUnitCount = SelectedUnits.Num();
	//if(LeftClickIsPressed)
	//{
	//	FHitResult Hit_CPoint;
	//	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit_CPoint);
	//	HUDBase->CPoint = Hit_CPoint.Location;
	//}
	
	//FHitResult Hit;
	//GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
	
	FHitResult Hit_Pawn;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit_Pawn);
	/*
	if(ExtendedCameraBase)
	{
		UUnitWidgetSelector* Selector = Cast<UUnitWidgetSelector>(ExtendedCameraBase->WidgetSelector);
		if(Selector)
		{
			Selector->SetVisibleButtonCount(SelectedUnits.Num());
			Selector->SetButtonLabelCount(SelectedUnits.Num());
		}
	}*/
	
	if(LeftClickIsPressed)
	{
		//FHitResult Hit_CPoint;
		//GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit_CPoint);
		HUDBase->CPoint = Hit_Pawn.Location;
	}
		
	AttackToggeledTick(Hit_Pawn);
	GravityToggeledTick(Hit_Pawn);
	LaserToggeledTick(Hit_Pawn);
	LockCamTick();
	CreateHoldPositionTick(Hit_Pawn);
	MouseBotTick(Hit_Pawn);
	DijkstraOnEnemyTick();

}
void AControllerBase::MouseBotTick(FHitResult Hit)
{
	HUDBase->GetAllMouseBots();
	TArray <AMouseBotBase*> MouseBots = HUDBase->MouseBots;
	
	if(MouseBots.Num())
		for (int32 i = 0; i < MouseBots.Num(); i++)
		{
			if(MouseBots[i])
			{
				HUDBase->ControllDirectionToMouse(MouseBots[i], Hit);
				MouseBots[i]->MoveToLocation = Hit.Location;
			}
		}
}

 void AControllerBase::DijkstraOnEnemyTick()
 {
	TArray<FPathPoint> PathPoints;
	if(!HUDBase->DisablePathFindingOnEnemy)
		for (int32 i = 0; i < HUDBase->EnemyUnitBases.Num(); i++)
			if( HUDBase->EnemyUnitBases[i] && ! HUDBase->EnemyUnitBases[i]->IsFriendly &&  HUDBase->EnemyUnitBases[i]->DijkstraSetPath)
			{
				SetRunLocationUseDijkstraForAI(HUDBase->EnemyUnitBases[i]->DijkstraEndPoint, HUDBase->EnemyUnitBases[i]->DijkstraStartPoint, HUDBase->EnemyUnitBases, PathPoints, i);
				HUDBase->EnemyUnitBases[i]->DijkstraSetPath = false;
			}
 }

 void AControllerBase::AttackToggeledTick(FHitResult Hit)
{

	if(AttackToggled)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			if(SelectedUnits[i] && SelectedUnits[i]->ToggleUnitDetection && SelectedUnits[i]->GetUnitState() == UnitData::Idle)
				HUDBase->ControllDirectionToMouse(SelectedUnits[i], Hit);


			AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		
			AUnitBase* UnitBase = Cast<AUnitBase>(Hit.GetActor());
			AScatterMine* Mine = Cast<AScatterMine>(Hit.GetActor());
			
			if(ExtendedUnitBase && (SelectedUnits[i]->GetUnitState() == UnitData::Attack || SelectedUnits[i]->GetUnitState() == UnitData::Pause || SelectedUnits[i]->GetUnitState() == UnitData::Idle ))
			{
				HUDBase->ControllDirectionToMouse(ExtendedUnitBase, Hit);
				if(UnitBase && Hit.bBlockingHit)
				{
					ExtendedUnitBase->MouseLocation = UnitBase->GetActorLocation();
	
				}else if(Mine && Hit.bBlockingHit)
				{
					ExtendedUnitBase->MouseLocation = Mine->GetActorLocation();
	
				}else
				{
					ExtendedUnitBase->MouseLocation = Hit.Location;
				}
				
				
			}
		}
	}
}

void AControllerBase::GravityToggeledTick(FHitResult Hit)
{
	if(GravityToggled)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			if(SelectedUnits[i] && SelectedUnits[i]->GetUnitState() == UnitData::UseGravity)
			{
				HUDBase->ControllDirectionToMouse(SelectedUnits[i], Hit);

				AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
				ExtendedUnitBase->MouseLocation = Hit.Location;
			}
		}
	}
}

void AControllerBase::LaserToggeledTick(FHitResult Hit)
{
	if(LaserToggled)
	{
		//bool LoadingFound = false;
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			if(SelectedUnits[i] && SelectedUnits[i]->GetUnitState() == UnitData::LaserLoading)
			{
				HUDBase->ControllDirectionToMouse(SelectedUnits[i], Hit);
				//LoadingFound = true;
			}
				
		}
		//LaserToggled = LoadingFound;
	}
}

void AControllerBase::LockCamTick()
{
	if (ExtendedCameraBase && SelectedUnits.Num() && LockCameraToUnit)
		ExtendedCameraBase->LockOnUnit(SelectedUnits[0]);
}

void AControllerBase::CreateHoldPositionTick(FHitResult Hit)
{
	if(DIsPressed)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			AHoldPositionUnit* HoldPositionUnit = Cast<AHoldPositionUnit>(SelectedUnits[i]);
			if(HoldPositionUnit && HoldPositionUnit->Detector)
			{
				FVector PointA = HoldPositionUnit->GetActorLocation();
				FVector PointB = Hit.Location;
				float XDistance = abs(PointB.X - PointA.X)/100.f;
				float YDistance = abs(PointB.Y - PointA.Y)/100.f;

				XDistance = XDistance > 1.f ? XDistance : 1.f;
				YDistance = YDistance > 1.f ? YDistance : 1.f;
				
				FVector ForwardVector = HoldPositionUnit->GetActorForwardVector();
			
				if (abs(ForwardVector.Y) < 0.5f)
				{
					float Buffer = XDistance;
					XDistance = YDistance;
					YDistance = Buffer;
				}

				float WeaponRange = HoldPositionUnit->Range;
				FVector Scaler = FVector(YDistance, XDistance, 1.f);
				HoldPositionUnit->Detector->ScaleDetectors(Scaler, WeaponRange);
				if(IsRightClickPressed)
					HUDBase->ControllDirectionToMouse(SelectedUnits[i], Hit);

			}
		}
	}
}

void AControllerBase::ShiftPressed()
{
	IsShiftPressed = true;
}

void AControllerBase::ShiftReleased()
{
	IsShiftPressed = false;
}

void AControllerBase::LeftClickPressed()
{
	
	LeftClickIsPressed = true;
	
	if (AttackToggled) {
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
			
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead && !ExtendedUnitBase) {     
				FHitResult Hit;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);


				FHitResult Hit_Pawn;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit_Pawn);

				if (Hit_Pawn.bBlockingHit)
				{
					AUnitBase* UnitBase = Cast<AUnitBase>(Hit_Pawn.GetActor());
					
					if(UnitBase && !UnitBase->IsFriendly)
					{
						/// Focus Enemy Units ///
						SelectedUnits[i]->UnitToChase = UnitBase;
						SelectedUnits[i]->SetUnitState(UnitData::Chase);
					}else
					{
						/// A-Move Units ///
						FVector RunLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);
						DrawDebugSphere(GetWorld(), RunLocation, 15, 5, FColor::Green, false, 1.5, 0, 1);

						SelectedUnits[i]->SetUnitState(UnitData::Run);
						SelectedUnits[i]->RunLocationArray.Empty();
						SelectedUnits[i]->RunLocationArrayIterator = 0;
						SelectedUnits[i]->RunLocation = RunLocation;
						SelectedUnits[i]->UnitStatePlaceholder = UnitData::Run;
					}
				}
				
			}
			
		}
		
	}
	else {

		FHitResult Hit_IPoint;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit_IPoint);
		HUDBase->IPoint = Hit_IPoint.Location;
		
		FHitResult Hit_Pawn;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit_Pawn);
		
		if (Hit_Pawn.bBlockingHit)
		{
			AUnitBase* UnitBase = Cast<AUnitBase>(Hit_Pawn.GetActor());
			const ASpeakingUnit* SUnit = Cast<ASpeakingUnit>(Hit_Pawn.GetActor());
			
			if (UnitBase && UnitBase->IsFriendly && (!SUnit || (SUnit && !SUnit->IsUnSelectable)))
			{
				HUDBase->SetUnitSelected(UnitBase);
				if(ExtendedCameraBase)
				{
					ExtendedCameraBase->SetTalentChooserLocation();
					ExtendedCameraBase->SetWidgetUnitSelectorLocation();
					ExtendedCameraBase->SetWeaponIndicatorLocation();
					ExtendedCameraBase->SetActionWidgetLocation();
				}
			}
			else {
				HUDBase->InitialPoint = HUDBase->GetMousePos2D();
				HUDBase->bSelectFriendly = true;
			}
		}
	}

}

void AControllerBase::LeftClickReleased()
{
	LeftClickIsPressed = false;
	HUDBase->bSelectFriendly = false;
	SetWidgets(0);
	
}
void AControllerBase::SelectUnit(int Index)
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		if(SelectedUnits[i] && i != Index) SelectedUnits[i]->SetDeselected();
	}
	SelectedUnits.Empty();
	SelectedUnits.Add(HUDBase->SelectedUnits[Index]);

}

void AControllerBase::SetWidgets(int Index)
{
	SelectedUnits = HUDBase->SelectedUnits;
	
	if (SelectedUnits.Num()) {

		if(CompassCamera && CompassCamera->CompassShell)
			CompassCamera->CompassShell->SetUnitBase(SelectedUnits[Index]);

		if(ExtendedCameraBase)
			ExtendedCameraBase->SetSelectorWidget(Index, SelectedUnits[Index]);

	
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[Index]);

		if(ExtendedUnitBase && ExtendedCameraBase)
		{
			ExtendedCameraBase->SetUserWidget(ExtendedUnitBase);
			//ExtendedCameraBase->SetSelectorWidget(Index, ExtendedUnitBase);
			SelectedUnitId = Index;
			USkeletalMesh* NewWeaponMeshIndicator = ExtendedUnitBase->GetWeaponMesh();
			if(NewWeaponMeshIndicator)
				ExtendedCameraBase->ChangeWeaponIndicator(NewWeaponMeshIndicator);
		}else
		{
			ExtendedCameraBase->SetUserWidget(nullptr);
			ExtendedCameraBase->ChangeWeaponIndicator(nullptr);
		}
	}
}

void AControllerBase::RightClickPressed()
{
	IsRightClickPressed = true;

	if(DIsPressed) return;
	
	if (SelectedUnits.Num())
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[0]);
		if(ExtendedUnitBase)
		{
			ExtendedUnitBase->ShowThrowRadius = false;
			ExtendedUnitBase->ShowGravityRadius = false;

		
			if(ExtendedUnitBase)
				ExtendedUnitBase->CreateIconNormalRadius();
			
			if(ExtendedUnitBase->GravityActor)
			{
				GravityToggled = false;
				ExtendedUnitBase->GravityActor->Mesh->SetSimulatePhysics(true);
			}
		}
	}

	
	AttackToggled = false;
	
	if (IsShiftPressed) {
		for (int32 i = 0; i < SelectedUnits.Num(); i++) {
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead) {
				FHitResult Hit;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
				FVector RunLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);
				DrawDebugSphere(GetWorld(), RunLocation, 15, 5, FColor::Green, false, 1.5, 0, 1);
				if(!SelectedUnits[i]->RunLocationArray.Num())
				{
					SelectedUnits[i]->RunLocation = RunLocation;
					SelectedUnits[i]->SetUnitState(UnitData::Run);
				}
				
				SelectedUnits[i]->RunLocationArray.Add(RunLocation);
				SelectedUnits[i]->UnitStatePlaceholder = UnitData::Idle;
				SelectedUnits[i]->UnitsToChase.Empty();
				SelectedUnits[i]->UnitToChase = nullptr;
				SelectedUnits[i]->ToggleUnitDetection = false;
			}
			
		}
	}
	else {
		TArray<FPathPoint> PathPoints;
		for (int32 i = 0; i < SelectedUnits.Num(); i++) {
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead) {
				FVector UnitLocation = SelectedUnits[i]->GetActorLocation();
				
				FHitResult Hit;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
				FVector RunLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

				
				DrawDebugSphere(GetWorld(), RunLocation, 15, 5, FColor::Green, false, 1.5, 0, 1);
				
				if(SelectedUnits[i]->GetUnitState() != UnitData::Run)
				SelectedUnits[i]->SetWalkSpeed(0.f);
				
				SelectedUnits[i]->SetUnitState(UnitData::Run);
				SelectedUnits[i]->RunLocationArray.Empty();
				SelectedUnits[i]->RunLocationArrayIterator = 0;
				SelectedUnits[i]->RunLocation = RunLocation;
				SelectedUnits[i]->UnitStatePlaceholder = UnitData::Idle;
				SelectedUnits[i]->ToggleUnitDetection = false;

				float Range = FVector::Dist(UnitLocation, Hit.Location);
				
				if(!HUDBase->DisablePathFindingOnFriendly && Range >= HUDBase->RangeThreshold && !HUDBase->IsLocationInNoPathFindingAreas(Hit.Location))
					SetRunLocationUseDijkstra(Hit.Location, UnitLocation, SelectedUnits, PathPoints, i);
			}
		}
	}
}

 void AControllerBase::RightClickReleased()
 {
	IsRightClickPressed = false;
 }

 void AControllerBase::SetRunLocationUseDijkstra(FVector HitLocation, FVector UnitLocation, TArray <AUnitBase*> Units, TArray<FPathPoint>& PathPoints, int i)
{
	HUDBase->SetNextDijkstraTo(Units, HitLocation);
	FHitResult LineHit;
	FVector LineEndLocation = FVector(HitLocation.X, HitLocation.Y, HitLocation.Z+10.f);
	GetWorld()->LineTraceSingleByChannel(LineHit, UnitLocation, LineEndLocation, HUDBase->TraceChannelProperty, HUDBase->QueryParams);
	
	float ZDistance_B = abs(Units[i]->Next_DijkstraPMatrixes.CenterPoint.Z - HitLocation.Z);
		
	if(LineHit.bBlockingHit && Units[i]->Next_DijkstraPMatrixes.Matrix.Num())
	{
		if(i == 0 && HUDBase->UseDijkstraOnlyOnFirstUnit)
		{
			if(ZDistance_B > 150.f) HUDBase->SetDijkstraWithClosestZDistance(Units[i], HitLocation.Z);
			PathPoints = HUDBase->GetPathReUseDijkstra( Units[i]->Next_DijkstraPMatrixes.Matrix, HitLocation, UnitLocation);
		}

		if(!HUDBase->UseDijkstraOnlyOnFirstUnit)
		{
			if(ZDistance_B > 150.f) HUDBase->SetDijkstraWithClosestZDistance(Units[i], HitLocation.Z);
			PathPoints = HUDBase->GetPathReUseDijkstra( Units[i]->Next_DijkstraPMatrixes.Matrix, HitLocation, UnitLocation);
		}
					
		if(PathPoints.Num())
		{
			Units[i]->RunLocation = PathPoints[0].Point + FVector(i / 2 * 50, i % 2 * 50, 0.f);;
					
			for(int k = 1; k < PathPoints.Num(); k++)
			{
				Units[i]->RunLocationArray.Add(PathPoints[k].Point);
			}
						
			Units[i]->RunLocationArray.Add(HitLocation + FVector(i / 2 * 50, i % 2 * 50, 0.f));
		}
	}
}

void AControllerBase::SetRunLocationUseDijkstraForAI(FVector HitLocation, FVector UnitLocation, TArray <AUnitBase*> Units, TArray<FPathPoint>& PathPoints, int i)
{
	FHitResult LineHit;
	FVector LineEndLocation = FVector(HitLocation.X, HitLocation.Y, HitLocation.Z+10.f);
	GetWorld()->LineTraceSingleByChannel(LineHit, UnitLocation, LineEndLocation, HUDBase->TraceChannelProperty, HUDBase->QueryParams);
	
	float ZDistance_B = abs(Units[i]->Next_DijkstraPMatrixes.CenterPoint.Z - HitLocation.Z);
		
	if(LineHit.bBlockingHit && Units[i]->Next_DijkstraPMatrixes.Matrix.Num())
	{
		
		PathPoints = HUDBase->GetPathReUseDijkstra( Units[i]->Next_DijkstraPMatrixes.Matrix, HitLocation, UnitLocation);
					
		if(PathPoints.Num())
		{
			Units[i]->RunLocation = PathPoints[0].Point;;
					
			for(int k = 1; k < PathPoints.Num(); k++)
			{
				Units[i]->RunLocationArray.Add(PathPoints[k].Point);
			}
						
			Units[i]->RunLocationArray.Add(HitLocation);
			Units[i]->FollowPath = true;
		}
	}else
	{
		Units[i]->FollowPath = false;
	}
}

void AControllerBase::APressed()
{
	if(!AttackToggled)
	{
		AttackToggled = true;
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead)
			{
				if(SelectedUnits[i])
				{
					SelectedUnits[i]->SetWalkSpeed(0);	
					SelectedUnits[i]->ToggleUnitDetection = true;
					AExtendedUnitBase* ExtendedUnit = Cast<AExtendedUnitBase>(SelectedUnits[i]);
					if(ExtendedUnit)
					{
						ExtendedUnit->SetUnitState(UnitData::Pause);
					}
				
					
					ALaserUnit* LaserUnit = Cast<ALaserUnit>(SelectedUnits[i]);
					
					if(LaserUnit)
						LaserUnit->SpawnLaserIndicator(LaserUnit->GetActorLocation()+ LaserUnit->GetActorForwardVector()*900.f+LaserUnit->SpawnOffset,FRotator(0.f,0.f,0.f));
					
				}
			}
		}
	}
}


void AControllerBase::AReleased()
{
		AttackToggled = false;
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead)
			{
				if(SelectedUnits[i])
				{
					SelectedUnits[i]->ToggleUnitDetection = false;

					AExtendedUnitBase* ExtendedUnit = Cast<AExtendedUnitBase>(SelectedUnits[i]);
					if(ExtendedUnit)
						ExtendedUnit->SetUnitState(UnitData::Idle);
					
					ALaserUnit* LaserUnit = Cast<ALaserUnit>(SelectedUnits[i]);
					if(LaserUnit)
						LaserUnit->DestroyLaserIndicator();
					
				}
			}
		}
}

void AControllerBase::SReleased()
{
	if (SelectedUnits.Num()) {
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[SelectedUnitId]);
		if(ExtendedUnitBase && !ExtendedUnitBase->UnitDidThrowWeapon)
		{
			ExtendedUnitBase->ActualWeaponId < ExtendedUnitBase->Weapons.Num() - 1 ? ExtendedUnitBase->ActualWeaponId += 1 : ExtendedUnitBase->ActualWeaponId = 0;
			ExtendedUnitBase->ChangeWeapon(ExtendedUnitBase->Weapons[ExtendedUnitBase->ActualWeaponId]);
			
			ATalentedUnit* TalentedUnit = Cast<ATalentedUnit>(SelectedUnits[SelectedUnitId]);
				if(TalentedUnit)
					TalentedUnit->SetTalentPoints(TalentedUnit->ActualWeaponId);
		}

		if(ExtendedUnitBase)
		{
			USkeletalMesh* NewWeaponMeshIndicator = ExtendedUnitBase->GetWeaponMesh();
			if(NewWeaponMeshIndicator)
				ExtendedCameraBase->ChangeWeaponIndicator(NewWeaponMeshIndicator);
		}
	}
}

void AControllerBase::RPressed()
{

	if (SelectedUnits.Num()) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[SelectedUnitId]);
		if(ExtendedUnitBase)
		{
			if(ExtendedUnitBase->GetUnitState() != UnitData::Reload && ExtendedUnitBase->GetUnitState() != UnitData::Pause)
			{
				ExtendedUnitBase->SetWalkSpeed(0);
				ExtendedUnitBase->UnitControlTimer = 0;
				ExtendedUnitBase->SetUnitState(UnitData::Reload);
			}
		}
	}
}

void AControllerBase::FPressed()
{
	AttackToggled = false;
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(!ExtendedUnitBase)
			return;
		
			FHitResult Hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
			ExtendedUnitBase->TeleportLocation = Hit.Location;
			
			ExtendedUnitBase->SetWalkSpeed(0);
			ExtendedUnitBase->UnitControlTimer = 0;
			ExtendedUnitBase->SetUnitState(UnitData::TeleportStart);
	
	}
}

void AControllerBase::CPressed()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(!ExtendedUnitBase)
			return;
		
			FHitResult Hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
			ExtendedUnitBase->ToggleUnitDetection = false;
			ExtendedUnitBase->ScatterMineSpawnLocation = Hit.Location;
			ExtendedUnitBase->UnitControlTimer = 0;
			ExtendedUnitBase->SetUnitState(UnitData::PrepareScatterMine);

	}
}

 void AControllerBase::DPressed()
 {
	DIsPressed = true;
	
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		AHoldPositionUnit* HoldPositionUnit = Cast<AHoldPositionUnit>(SelectedUnits[i]);
		if(HoldPositionUnit)
		{
			if(HoldPositionUnit->Detector)
			HoldPositionUnit->Detector->Destroy(true);
			HoldPositionUnit->SetUnitState(UnitData::HoldPositionCreate);
			HoldPositionUnit->SpawnDetector();
		}
	}
 }



void AControllerBase::DReleased()
{
	DIsPressedState = 1;
	DIsPressed = false;
	
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		AHoldPositionUnit* HoldPositionUnit = Cast<AHoldPositionUnit>(SelectedUnits[i]);
		if(HoldPositionUnit)
		{
			HoldPositionUnit->SetUnitState(UnitData::HoldPositionWatch);
			HoldPositionUnit->DetectorUnLock();
		}
	}
}

 void AControllerBase::EPressed()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		if(ExtendedUnitBase)
		ExtendedUnitBase->ToggleShield();
	}
}

void AControllerBase::XPressed()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(!ExtendedUnitBase)
			return;
		
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		ExtendedUnitBase->ToggleUnitDetection = false;
		ExtendedUnitBase->MouseBotSpawnLocation = Hit.Location;
		ExtendedUnitBase->UnitControlTimer = 0;
		ExtendedUnitBase->SetUnitState(UnitData::PrepareMouseBot);
	}
}

void AControllerBase::WPressed()
{
	if(SelectedUnits.Num() && SelectedUnits[0])
	{
		ALaserUnit* LaserUnit = Cast<ALaserUnit>(SelectedUnits[0]);
	
		if(!LaserUnit)
			return;
	
		if(!LaserUnit->ActiveLaserIndicator)
		{
			LaserUnit->SetUnitState(UnitData::LaserLoading);
			LaserUnit->SpawnLaserIndicator(LaserUnit->GetActorLocation()+ LaserUnit->GetActorForwardVector()*900.f+LaserUnit->SpawnOffset,FRotator(0.f,0.f,0.f));
			LaserToggled = true;
		}else
		{
			LaserUnit->SetUnitState(UnitData::Idle);
			LaserUnit->DestroyLaserIndicator();
			LaserToggled = false;
		}
	}
}

 void AControllerBase::WReleased()
 {

 }

 void AControllerBase::YPressed()
{
	if(GravityToggled)
	{
		if(SelectedUnits.Num())
		{
			AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[0]);
			if(ExtendedUnitBase && ExtendedUnitBase->GetUnitState() == UnitData::UseGravity)
			{
				GravityToggled = false;
			
				if(ExtendedUnitBase->GravityActor)
					ExtendedUnitBase->GravityActor->Mesh->SetSimulatePhysics(true);
			
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}
		}
	}else if(!GravityToggled)
	{
		FHitResult Hit_Actor;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit_Actor);
	
		if (Hit_Actor.bBlockingHit)
		{
			AGravityActor* GravityActor = Cast<AGravityActor>(Hit_Actor.GetActor());
		
			if (GravityActor)
			{
				AScatterMine* ScatterMine = Cast<AScatterMine>(GravityActor);

				if(ScatterMine && !ScatterMine->IsFriendly)
					return;
			
				if(ScatterMine && ScatterMine->IsExplodableWhileGravity)
					ScatterMine->IsExplodable = true;
			
				for (int32 i = 0; i < SelectedUnits.Num(); i++)
				{
				
					AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
				
					if(ExtendedUnitBase)
					{
						float GravityRange = ExtendedUnitBase->GravityRange;
						FVector VectorDiff =  GravityActor->GetActorLocation() - ExtendedUnitBase->GetActorLocation();
						float RangeDiff = sqrt(VectorDiff.X*VectorDiff.X +VectorDiff.Y*VectorDiff.Y +VectorDiff.Z*VectorDiff.Z);
					
						if( RangeDiff <= GravityRange )
						{
							ExtendedUnitBase->GravityActor = GravityActor;
							ExtendedUnitBase->SetUnitState(UnitData::UseGravity);
							GravityToggled = true;
						}else
						{
							ExtendedUnitBase->ShowGravityRadius = true;
						}
					}
				}
			}
		}
	}
}

void AControllerBase::TPressed()
{
	
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		if(ExtendedUnitBase && ExtendedUnitBase->GetUnitState() != UnitData::ThrowWeapon && !ExtendedUnitBase->WeaponUnlocked && ExtendedUnitBase->Weapon)
		{
			FHitResult Hit_Actor;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit_Actor);
	
			if (Hit_Actor.bBlockingHit && ExtendedUnitBase->Weapon)
			{
				AUnitBase* UnitBase = Cast<AUnitBase>(Hit_Actor.GetActor());

				if(UnitBase && !UnitBase->IsFriendly)
				{
					float ThrowRange = ExtendedUnitBase->ThrowRange;

					FVector VectorDiff =  UnitBase->GetActorLocation() - ExtendedUnitBase->GetActorLocation();
					float RangeDiff = sqrt(VectorDiff.X*VectorDiff.X +VectorDiff.Y*VectorDiff.Y +VectorDiff.Z*VectorDiff.Z);
				
					if (RangeDiff <= ThrowRange )
					{
						for(int x = 0; x < 10; x++)HUDBase->ControllDirectionToMouse(SelectedUnits[i], Hit_Actor);
						
						ExtendedUnitBase->UnitToThrowWeapon = UnitBase;
						ExtendedUnitBase->SetWeaponFlying();
						ExtendedUnitBase->UnitControlTimer = 0;
						
						ExtendedUnitBase->UnitDidThrowWeapon = true;
						ExtendedUnitBase->SetUnitState(UnitData::ThrowWeapon);
					
					}else
					{
						ExtendedUnitBase->ShowThrowRadius = true;
					}
				}
			}
		}else if(ExtendedUnitBase)
		{
			
			ExtendedUnitBase->SetWeaponReturning();
			ExtendedUnitBase->UnitControlTimer = 0;
			ExtendedUnitBase->SetUnitState(UnitData::WeaponGravity);
			return;
		}
	}
	



}


void AControllerBase::QPressed()
{
	QIsPressed = true;
	
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(!ExtendedUnitBase) return;
		
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		ExtendedUnitBase->JumpCounter++;

		if(ExtendedUnitBase->JumpCounter <= ExtendedUnitBase->MaxJumps)
		{
			FVector From = ExtendedUnitBase->GetActorLocation();
			FVector To = Hit.Location;
			
			FVector DirectionVector = UKismetMathLibrary::GetDirectionUnitVector(From,  To);
			ExtendedUnitBase->SetUnitState(UnitData::Jump);
			ExtendedUnitBase->LaunchCharacter(DirectionVector*ExtendedUnitBase->JumpLoad,false, false);
		}
	}
}

void AControllerBase::QReleased()
{
	QIsPressed = false;
}

void AControllerBase::JumpCamera()
{
	if (ExtendedCameraBase && (IsStrgPressed || IsSpacePressed))
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		ExtendedCameraBase->JumpCamera(Hit);

	}
}


void AControllerBase::StrgPressed() {
	IsStrgPressed = true;
}

void AControllerBase::StrgReleased() {
	IsStrgPressed = false;
}

void AControllerBase::ZoomIn()
{
	if (ExtendedCameraBase && IsStrgPressed)
	{
		ExtendedCameraBase->ZoomIn(1.f);
	}
}

void AControllerBase::ZoomOut()
{
	if (ExtendedCameraBase && IsStrgPressed)
	{
		ExtendedCameraBase->ZoomOut(1.f);
	}
}




void AControllerBase::MouseBotStateMachine(float DeltaSeconds)
{
	TArray <AMouseBotBase*> MouseBots = HUDBase->MouseBots;
	
	if(MouseBots.Num())
	
		for (int32 i = 0; i < MouseBots.Num(); i++) {
				
			if(MouseBots[i])
				switch (MouseBots[i]->GetAnimState())
				{
			case BotData::Idle_Bot:
				{
					FHitResult Hit;
					GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
					HUDBase->ControllDirectionToMouse(MouseBots[i], Hit);
				}
					break;
			case BotData::Run_Bot:
				{
					FHitResult Hit;
					GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
					HUDBase->ControllDirectionToMouse(MouseBots[i], Hit);
					MouseBots[i]->MoveToLocation = Hit.Location;
				}
					break;
			case BotData::Dead_Bot:
				{
					if(MouseBots[i]->DeathTime > 5.f)
					{
						AMouseBotBase* BotToKill = MouseBots[i];
						HUDBase->RemoveMouseBot(BotToKill);
						BotToKill->KillBot();
					}
								
				}
					break;
			default:
				{
							
				}
					break;
				}
		}
}

void AControllerBase::AltPressed()
{
	TArray <AMouseBotBase*> MouseBots = HUDBase->MouseBots;
	
	for(int i = 0; i < MouseBots.Num(); i++)
	{
		if(MouseBots[i])
		{
			if(!BotIsToggled)
			{
				MouseBots[i]->SetAnimState(BotData::Run_Bot);
			}else
			{
				MouseBots[i]->SetAnimState(BotData::Idle_Bot);
			}	
		}
	}
	BotIsToggled = !BotIsToggled;
}

void AControllerBase::AltReleased()
{
	AltIsPressed = false;
}