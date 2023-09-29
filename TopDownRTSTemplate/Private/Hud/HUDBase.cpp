// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Hud/HUDBase.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Elements/Actor/ActorElementData.h"
#include "GeometryCollection/GeometryCollectionSimulationTypes.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"


void AHUDBase::DrawHUD()
{

	if (bSelectFriendly) {

		if(CharacterIsUnSelectable)
		DeselectAllUnits();

		if(CharacterIsUnSelectable)
		SelectedUnits.Empty();
		
		CurrentPoint = GetMousePos2D();

		if (abs(InitialPoint.X - CurrentPoint.X) >= 2) {

			DrawRect(FLinearColor(0, 0, 1, .15f),
				InitialPoint.X,
				InitialPoint.Y,
				CurrentPoint.X - InitialPoint.X,
				CurrentPoint.Y - InitialPoint.Y);

			const float LengthLineA = abs(InitialPoint.Y - CurrentPoint.Y);
			const float LengthLineB = abs(InitialPoint.X - CurrentPoint.X);
			FVector2D LineCenterPointA;
			FVector2D LineCenterPointB;
			FVector2D InitialSelectionPoint;
			FVector2D CurrentSelectionPoint;

			if (InitialPoint.Y < CurrentPoint.Y && InitialPoint.X < CurrentPoint.X) {
				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y + (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X - (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);

			}
			else if (InitialPoint.Y < CurrentPoint.Y && InitialPoint.X > CurrentPoint.X) {

				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y + (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X + (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);

			}
			else if (InitialPoint.Y > CurrentPoint.Y && InitialPoint.X < CurrentPoint.X) {
				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y - (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X - (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);
			}
			else if (InitialPoint.Y > CurrentPoint.Y && InitialPoint.X > CurrentPoint.X) {
				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y - (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X + (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);
			}


			DrawRect(FLinearColor(0, 1, 0, .15f),
				InitialSelectionPoint.X,
				InitialSelectionPoint.Y,
				CurrentSelectionPoint.X - InitialSelectionPoint.X,
				CurrentSelectionPoint.Y - InitialSelectionPoint.Y);


			
			TArray <AUnitBase*> NewUnitBases;
			NewUnitBases.Empty();
			GetActorsInSelectionRectangle<AUnitBase>(InitialSelectionPoint, CurrentSelectionPoint, NewUnitBases, false, false);
			//UE_LOG(LogTemp, Warning, TEXT("NewUnitBases: %d"), NewUnitBases.Num());
			if(NewUnitBases.Num() && !CharacterIsUnSelectable)
			{
				if(NewUnitBases.Num())
				{
					for (int32 i = 0; i < SelectedUnits.Num(); i++)
					{
						if(SelectedUnits[i])
						SelectedUnits[i]->SetDeselected();
					}
					SelectedUnits.Empty();
				}
				
				for (int32 i = 0; i < NewUnitBases.Num(); i++) {

					//FVector ALocation = NewUnitBases[i]->GetActorLocation();
					const ASpeakingUnit* SUnit = Cast<ASpeakingUnit>(NewUnitBases[i]);
					//&& (!SUnit || (SUnit && !SUnit->IsUnSelectable))
					if(NewUnitBases[i]->IsFriendly && (!SUnit || (SUnit && !SUnit->IsUnSelectable))) // && IsActorInsideRec(IPoint, CPoint, ALocation) // && IsActorInsideRec(IPoint, CPoint, ALocation)
					{
						NewUnitBases[i]->SetSelected();
						SelectedUnits.Add(NewUnitBases[i]);
					}
				}
			}else if(CharacterIsUnSelectable)
			{
				if(SelectedUnits.Num())
				{
					for (int32 i = 0; i < SelectedUnits.Num(); i++)
					{
						SelectedUnits[i]->SetDeselected();
					}
					SelectedUnits.Empty();
				}
			}
			NewUnitBases.Empty();
			

		}

	}
}

void AHUDBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MoveUnitsThroughWayPoints(FriendlyUnits);
	IsSpeakingUnitClose(FriendlyUnits, SpeakingUnits);
}

void AHUDBase::BeginPlay()
{
	Super::BeginPlay();
	GetAllMouseBots();
	for(int i = 0; i < AllUnits.Num(); i++)
	{
		AUnitBase* Unit = Cast<AUnitBase>(AllUnits[i]);
		if(Unit->IsFriendly)
			FriendlyUnits.Add(Unit);

		ASpeakingUnit* SpeakingUnit = Cast<ASpeakingUnit>(AllUnits[i]);
		if(SpeakingUnit)
			SpeakingUnits.Add(SpeakingUnit);

		
			
	}
}

void AHUDBase::GetAllMouseBots()
{
	TArray <AActor*> AllActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMouseBotBase::StaticClass(), AllActors);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnitBase::StaticClass(), AllActors);
	for(int i = 0; i < AllActors.Num(); i++)
	{
		// Filter Actors
		MouseBots.Emplace(Cast<AMouseBotBase>(AllActors[i]));
	}
}

FVector2D AHUDBase::GetMousePos2D()
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	return FVector2D(PosX, PosY);
}

void AHUDBase::RemoveMouseBot(AMouseBotBase* MouseBotBase)
{
	MouseBots.Remove(MouseBotBase);
}

void AHUDBase::MoveUnitsThroughWayPoints(TArray <AUnitBase*> Units)
{
	for (int32 i = 0; i < Units.Num(); i++) {

		if(Units[i])
			if (Units[i]->RunLocationArray.Num()) {

				FVector ActorLocation = Units[i]->GetActorLocation();
				const float Distance = sqrt((ActorLocation.X-Units[i]->RunLocation.X)*(ActorLocation.X-Units[i]->RunLocation.X)+(ActorLocation.Y-Units[i]->RunLocation.Y)*(ActorLocation.Y-Units[i]->RunLocation.Y));//FVector::Distance(ActorLocation, Units[i]->RunLocation);
;
				if (Distance <= Units[i]->StopRunTolerance) { // || DistanceY <= Units[i]->StopRunToleranceY 
					if (Units[i]->RunLocationArrayIterator < Units[i]->RunLocationArray.Num()) {
					
						Units[i]->RunLocation = Units[i]->RunLocationArray[Units[i]->RunLocationArrayIterator];
						Units[i]->SetUnitState(UnitData::Run);
					}
					else {
						Units[i]->RunLocationArray.Empty();
						Units[i]->RunLocationArrayIterator = 0;
					}
					Units[i]->RunLocationArrayIterator++;
				}
			}


	}
}

void AHUDBase::IsSpeakingUnitClose(TArray <AUnitBase*> Units, TArray <ASpeakingUnit*> SpeakUnits)
{
	for (int32 i = 0; i < SpeakUnits.Num(); i++)
	{
	
		
		if(SpeakUnits[i] && SpeakUnits[i]->IsUnSelectable){
			bool FoundUnit = false;
			for (int32 u = 0; u < Units.Num(); u++) {

				const ASpeakingUnit* SUnit = Cast<ASpeakingUnit>(Units[u]);
				
				if(Units[u] && Units[u]->IsFriendly && SUnit != SpeakUnits[i])
				{
					const FVector UnitLocation = Units[u]->GetActorLocation();
					const FVector SpeakingUnitLocation = SpeakUnits[i]->GetActorLocation();
					const float MinSpeakDistance = SpeakUnits[i]->MinSpeakDistance;
					const float Distance = sqrt((UnitLocation.X-SpeakingUnitLocation.X)*(UnitLocation.X-SpeakingUnitLocation.X)+(UnitLocation.Y-SpeakingUnitLocation.Y)*(UnitLocation.Y-SpeakingUnitLocation.Y));
					
					if(Distance < MinSpeakDistance && SpeakUnits[i]->SpeechBubble && SpeakUnits[i]->SpeechBubble->WidgetIndex != 2)
					{
						SpeakUnits[i]->SpeechBubble->WidgetIndex = 1;
						SpeakUnits[i]->SpeechBubble->WidgetSwitcher->SetActiveWidgetIndex(1);
						FoundUnit = true;
					}else if(SpeakUnits[i]->SpeechBubble && SpeakUnits[i]->SpeechBubble->WidgetIndex != 2 && !FoundUnit)
					{
						SpeakUnits[i]->SpeechBubble->WidgetIndex = 0;
						SpeakUnits[i]->SpeechBubble->WidgetSwitcher->SetActiveWidgetIndex(0);
					}
				}
			}
		}
	}
}


void AHUDBase::PatrolUnitsThroughWayPoints(TArray <AUnitBase*> Units)
{
	for (int32 i = 0; i < Units.Num(); i++) {

		if(Units[i])
			if (Units[i]->RunLocationArray.Num()) {

				FVector ActorLocation = Units[i]->GetActorLocation();

				const float Distance = sqrt((ActorLocation.X-Units[i]->RunLocation.X)*(ActorLocation.X-Units[i]->RunLocation.X)+(ActorLocation.Y-Units[i]->RunLocation.Y)*(ActorLocation.Y-Units[i]->RunLocation.Y));//FVector::Distance(ActorLocation, Units[i]->RunLocation);

				if (Distance <= Units[i]->StopRunTolerance) { // || DistanceY <= Units[i]->StopRunToleranceY 
					if (Units[i]->RunLocationArrayIterator < Units[i]->RunLocationArray.Num()) {
					
						Units[i]->RunLocation = Units[i]->RunLocationArray[Units[i]->RunLocationArrayIterator];
						Units[i]->SetUnitState(UnitData::Patrol);
					}
					else {
						Units[i]->RunLocationArray.Empty();
						Units[i]->RunLocationArrayIterator = 0;
					}
					Units[i]->RunLocationArrayIterator++;
				}
			}


	}
}

void AHUDBase::SetUnitSelected(AUnitBase* Unit)
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		SelectedUnits[i]->SetDeselected();
	}

	SelectedUnits.Empty();
	SelectedUnits.Add(Unit);

	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		SelectedUnits[i]->SetSelected();
	}
}

void AHUDBase::DeselectAllUnits()
{
	if(CharacterIsUnSelectable)
		for (int32 i = 0; i < SelectedUnits.Num(); i++) {
			if(SelectedUnits[i])
				SelectedUnits[i]->SetDeselected();
		}

	if(CharacterIsUnSelectable)
		SelectedUnits.Empty();
}

void AHUDBase::ControllDirectionToMouse(AActor* Unit, FHitResult Hit)
{
	FVector CharacterDirectionVector = Unit->GetActorForwardVector();
	FVector ActorLocation = Unit->GetActorLocation();
	float Distance = FVector::Distance(ActorLocation, Hit.Location);
	float AngleDiff = CalculateAngle(CharacterDirectionVector, FVector(Hit.Location.X, Hit.Location.Y, ActorLocation.Z) - ActorLocation);
	float PreRotator;
	
	if(FMath::Abs(AngleDiff) < 0.01f) return;
	
	PreRotator = FMath::Abs(AngleDiff/ 5.0f)+(FMath::Pow(AngleDiff/ 20.0f, 8));

	
	float DistanceMultiplier = FMath::Clamp(FMath::Pow(Distance / 500.f, 2.f), 0.f, 1.f);
	float Inverter = AngleDiff < 0.f ? -1.f : 1.f;
	float RotationValue = FMath::Clamp(DistanceMultiplier * PreRotator *Inverter, -50.0f, 50.0f);

	FQuat QuadRotation = FQuat(FRotator(0.f, RotationValue, 0.f));


	Unit->AddActorLocalRotation(QuadRotation, false, 0, ETeleportType::None);
}


bool AHUDBase::IsActorInsideRec(FVector InPoint, FVector CuPoint, FVector ALocation)
{	
	if(InPoint.X < ALocation.X && ALocation.X < CuPoint.X && InPoint.Y < ALocation.Y && ALocation.Y < CuPoint.Y) return true;
	if(InPoint.X > ALocation.X && ALocation.X > CuPoint.X && InPoint.Y > ALocation.Y && ALocation.Y > CuPoint.Y) return true;
	if(InPoint.X < ALocation.X && ALocation.X < CuPoint.X && InPoint.Y > ALocation.Y && ALocation.Y > CuPoint.Y) return true;
	if(InPoint.X > ALocation.X && ALocation.X > CuPoint.X && InPoint.Y < ALocation.Y && ALocation.Y < CuPoint.Y) return true;

	return false;
}

