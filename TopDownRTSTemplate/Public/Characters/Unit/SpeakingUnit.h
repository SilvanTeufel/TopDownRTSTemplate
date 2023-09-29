// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "FogExplorer.h"
#include "UnitBase.h"
#include "Widgets/SpeechBubble.h"
#include "Components/AudioComponent.h"
#include "Widgets/TriggeredSpeech.h"
#include "SpeakingUnit.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ASpeakingUnit : public AUnitBase
{
	GENERATED_BODY()
public:
	ASpeakingUnit(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	bool IsResponsive = true;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void PlaySoundOnce(float Timer, UAudioComponent*& Component, USoundBase* Sound, float Volume);

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetSpeechWidgetText();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetTriggeredSpeechText();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void StartTriggeredSpeechText();

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void SetSpeechWidgetLocation(FVector NewLocation);
public:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechBubbleWidgetComp", Keywords = "RTSUnitTemplate SpeechBubbleWidgetComp"), Category = RTSUnitTemplate)
	class UWidgetComponent* SpeechBubbleWidgetComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechBubbleWidgetCompLocation", Keywords = "RTSUnitTemplate SpeechBubbleWidgetCompLocation"), Category = RTSUnitTemplate)
	FVector SpeechBubbleWidgetCompLocation = FVector (0.f, 0.f, 220.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechBubble", Keywords = "RTSUnitTemplate SpeechBubble"), Category = RTSUnitTemplate)
	USpeechBubble* SpeechBubble;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechBubbleWidgetCompLocation", Keywords = "RTSUnitTemplate SpeechBubbleWidgetCompLocation"), Category = RTSUnitTemplate)
	FVector TriggeredSpeechWidgetCompLocation = FVector (0.f, 0.f, 200.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	class UWidgetComponent* TriggeredSpeechWidgetComp;
	
	UPROPERTY(EditAnywhere, Category = RTSUnitTemplate)
	UTriggeredSpeech* TriggeredSpeech;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LockCamOnUnit", Keywords = "RTSUnitTemplate LockCamOnUnit"), Category = RTSUnitTemplate)
	bool LockCamOnUnit = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeakZoomPosition", Keywords = "RTSUnitTemplate SpeakZoomPosition"), Category = RTSUnitTemplate)
	float SpeakZoomPosition = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpeechVolume", Keywords = "RTSUnitTemplate SpeechVolume"), Category = RTSUnitTemplate)
	float SpeechVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "BackgroundVolume", Keywords = "RTSUnitTemplate BackgroundVolume"), Category = RTSUnitTemplate)
	float BackgroundVolume = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MinSpeakDistance", Keywords = "RTSUnitTemplate MinSpeakDistance"), Category = RTSUnitTemplate)
	float MinSpeakDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = RTSUnitTemplate)
	bool IsUnSelectable = false;
	
	int Text_Id = 1;
	

	
};
