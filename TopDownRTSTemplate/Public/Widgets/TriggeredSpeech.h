// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TriggeredSpeech.generated.h"
/**
 * 
 */
UCLASS()
class TOPDOWNRTSTEMPLATE_API UTriggeredSpeech : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock;

	UPROPERTY(EditAnywhere, Category = "Triggered Speech")
	FText SpeechText;

	UPROPERTY(EditAnywhere, Category = "Triggered Speech")
	float TextSpeed = 0.1f; // Adjust the speed at which characters appear

	UPROPERTY(EditAnywhere, Category = "Triggered Speech")
	float ClearDelay = 4.0f;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void StartTextGeneration();
	
private:
	FTimerHandle TimerHandle;
	int32 CurrentIndex;

	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void UpdateText();
	
	UFUNCTION(BlueprintCallable, Category = TopDownRTSTemplate)
	void ClearText();
};
