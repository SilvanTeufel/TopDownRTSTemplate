// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Controller/ControllerBase.h"
#include "Characters/Unit/SpeakingUnit.h"
#include "Controller/Input/EnhancedInputComponentBase.h"
#include "CameraControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API ACameraControllerBase : public AControllerBase
{
	GENERATED_BODY()
	
	ACameraControllerBase();

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CheckSpeakingUnits", Keywords = "TopDownRTSCamLib CheckSpeakingUnits"), Category = RTSUnitTemplate)
	bool CheckSpeakingUnits();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSCamLib)
	void SetResolutionTick(int x);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetViewPortScreenSizes", Keywords = "TopDownRTSCamLib GetViewPortScreenSizes"), Category = TopDownRTSCamLib)
	void GetViewPortScreenSizes(int x);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCameraPanDirection", Keywords = "TopDownRTSCamLib GetCameraPanDirection"), Category = TopDownRTSCamLib)
	FVector GetCameraPanDirection();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetCameraZDistance", Keywords = "TopDownRTSCamLib SetCameraZDistance"), Category = RTSUnitTemplate)
	void SetCameraZDistance(int Index);
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void RotateCam(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CameraBaseMachine", Keywords = "TopDownRTSCamLib CameraBaseMachine"), Category = TopDownRTSCamLib)
	void CameraBaseMachine(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ToggleLockCamToCharacter", Keywords = "TopDownRTSCamLib ToggleLockCamToCharacter"), Category = TopDownRTSCamLib)
	void ToggleLockCamToCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnlockCamFromCharacter", Keywords = "TopDownRTSCamLib UnlockCamFromCharacter"), Category = TopDownRTSCamLib)
	void UnlockCamFromCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToSpecificUnit", Keywords = "TopDownRTSCamLib LockCamToSpecificUnit"), Category = RTSUnitTemplate)
	void LockCamToSpecificUnit(AUnitBase* SUnit);

	ASpeakingUnit* SpeakingUnit;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToCharacter", Keywords = "TopDownRTSCamLib LockCamToCharacter"), Category = RTSUnitTemplate)
	void LockCamToCharacter(int Index);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToCharacter", Keywords = "TopDownRTSCamLib LockCamToCharacter"), Category = TopDownRTSCamLib)
	void LockZDistanceToCharacter();
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "LockCameraToCharacter", Keywords = "TopDownRTSCamLib LockCameraToCharacter"), Category = TopDownRTSCamLib)
	bool LockCameraToCharacter = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsRotatingRight", Keywords = "TopDownRTSCamLib CamIsRotatingRight"), Category = TopDownRTSCamLib)
	bool CamIsRotatingRight = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsRotatingLeft", Keywords = "TopDownRTSCamLib CamIsRotatingLeft"), Category = TopDownRTSCamLib)
	bool CamIsRotatingLeft = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsZoomingInState", Keywords = "TopDownRTSCamLib CamIsZoomingInState"), Category = RTSUnitTemplate)
	int CamIsZoomingInState = 0;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsZoomingOutState", Keywords = "TopDownRTSCamLib CamIsZoomingOutState"), Category = RTSUnitTemplate)
	int CamIsZoomingOutState = 0;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ZoomOutToPosition", Keywords = "TopDownRTSCamLib ZoomOutToPosition"), Category = TopDownRTSCamLib)
	bool ZoomOutToPosition = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ZoomInToPosition", Keywords = "TopDownRTSCamLib ZoomInToPosition"), Category = TopDownRTSCamLib)
	bool ZoomInToPosition = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "HoldZoomOnLockedCharacter", Keywords = "TopDownRTSCamLib HoldZoomOnLockedCharacter"), Category = TopDownRTSCamLib)
	bool HoldZoomOnLockedCharacter = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ScrollZoomCount", Keywords = "TopDownRTSCamLib ScrollZoomCount"), Category = RTSUnitTemplate)
	float ScrollZoomCount = 0.f;
};
