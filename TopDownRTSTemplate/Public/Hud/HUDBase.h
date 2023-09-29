// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/SpeakingUnit.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/MouseBotBase.h"
#include "HUDBase.generated.h"



/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API AHUDBase : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawHUD", Keywords = "TopDownRTSTemplate DrawHUD"), Category = TopDownRTSTemplate)
		virtual void DrawHUD(); // used in Tick();

	void Tick(float DeltaSeconds);

	void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "bSelectFriendly", Keywords = "TopDownRTSTemplate bSelectFriendly"), Category = TopDownRTSTemplate)
	bool bSelectFriendly = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "InitialPoint", Keywords = "TopDownRTSTemplate InitialPoint"), Category = TopDownRTSTemplate)
		FVector2D InitialPoint; // Position of mouse on screen when pressed;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CurrentPoint", Keywords = "TopDownRTSTemplate CurrentPoint"), Category = TopDownRTSTemplate)
		FVector2D CurrentPoint; // Position of mouse on screen while holding;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "IPoint", Keywords = "TopDownRTSTemplate IPoint"), Category = TopDownRTSTemplate)
		FVector IPoint = FVector(0.f,0.f, 0.f); // Position of mouse on screen when pressed;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CPoint", Keywords = "TopDownRTSTemplate CPoint"), Category = TopDownRTSTemplate)
		FVector CPoint = FVector(0.f,0.f, 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "RectangleScaleSelectionFactor", Keywords = "TopDownRTSTemplate RectangleScaleSelectionFactor"), Category = TopDownRTSTemplate)
		float RectangleScaleSelectionFactor = 0.9;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMousePos2D", Keywords = "TopDownRTSTemplate GetMousePos2D"), Category = TopDownRTSTemplate)
		FVector2D GetMousePos2D();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedEnemyUnits", Keywords = "TopDownRTSTemplate SelectedEnemyUnits"), Category = TopDownRTSTemplate)
	    TArray <AUnitBase*> SelectedUnits;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedEnemyUnits", Keywords = "TopDownRTSTemplate SelectedEnemyUnits"), Category = TopDownRTSTemplate)
		TArray <AUnitBase*> FriendlyUnits;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "AllUnits", Keywords = "TopDownRTSTemplate AllUnits"), Category = TopDownRTSTemplate)
		TArray <AActor*> AllUnits;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "EnemyUnitBases", Keywords = "TopDownRTSTemplate EnemyUnitBases"), Category = TopDownRTSTemplate)
		TArray <AUnitBase*> EnemyUnitBases;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "FoundMouseBots", Keywords = "TopDownRTSTemplate FoundMouseBots"), Category = TopDownRTSTemplate)
		TArray <AMouseBotBase*> MouseBots;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "AllUnitBases", Keywords = "TopDownRTSTemplate AllUnitBases"), Category = TopDownRTSTemplate)
		TArray <AUnitBase*> AllUnitBases;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SpeakingUnits", Keywords = "RTSUnitTemplate SpeakingUnits"), Category = RTSUnitTemplate)
	TArray <ASpeakingUnit*> SpeakingUnits;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsSpeakingUnitClose", Keywords = "RTSUnitTemplate IsSpeakingUnitClose"), Category = TopDownRTSTemplate)
		void IsSpeakingUnitClose(TArray <AUnitBase*> Units, TArray <ASpeakingUnit*> SpeakUnits);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RemoveMouseBot", Keywords = "TopDownRTSTemplate RemoveMouseBot"), Category = TopDownRTSTemplate)
		void RemoveMouseBot(AMouseBotBase* MouseBotBase);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CharacterIsUnSelectable", Keywords = "TopDownRTSTemplate CharacterIsUnSelectable"), Category = TopDownRTSTemplate)
		bool CharacterIsUnSelectable = false;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveUnitsThroughWayPoints", Keywords = "TopDownRTSTemplate MoveUnitsThroughWayPoints"), Category = TopDownRTSTemplate)
		void MoveUnitsThroughWayPoints(TArray <AUnitBase*> Units);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PatrolUnitsThroughWayPoints", Keywords = "TopDownRTSTemplate PatrolUnitsThroughWayPoints"), Category = TopDownRTSTemplate)
		void PatrolUnitsThroughWayPoints(TArray <AUnitBase*> Units);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetUnitSelected", Keywords = "TopDownRTSTemplate SetUnitSelected"), Category = TopDownRTSTemplate)
		void SetUnitSelected(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DeselectAllUnits", Keywords = "TopDownRTSTemplate DeselectAllUnits"), Category = TopDownRTSTemplate)
		void DeselectAllUnits();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAllMouseBots", Keywords = "TopDownRTSTemplate GetAllMouseBots"), Category = TopDownRTSTemplate)
		void GetAllMouseBots();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ControllDirectionToMouse", Keywords = "TopDownRTSTemplate ControllDirectionToMouse"), Category = TopDownRTSTemplate)
		void ControllDirectionToMouse(AActor* Unit, FHitResult Hit);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsActorInsideRectangle", Keywords = "TopDownRTSTemplate IsActorInsideRectangle"), Category = TopDownRTSTemplate)
		bool IsActorInsideRec(FVector InPoint, FVector CuPoint, FVector ALocation);




	
};
