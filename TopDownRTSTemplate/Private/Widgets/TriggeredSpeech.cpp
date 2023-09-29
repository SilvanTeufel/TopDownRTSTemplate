// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/TriggeredSpeech.h"
#include "TimerManager.h"

void UTriggeredSpeech::NativeConstruct()
{
	Super::NativeConstruct();
	ClearText();
	//UpdateText();
}

void UTriggeredSpeech::UpdateText()
{

	if (TextBlock)
	{
		FString DisplayText = SpeechText.ToString().Left(CurrentIndex);
		TextBlock->SetText(FText::FromString(DisplayText));

		// Increment the index to display the next character
		CurrentIndex++;

		if (CurrentIndex <= SpeechText.ToString().Len())
		{
			// Schedule the next update
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTriggeredSpeech::UpdateText, TextSpeed, false);
		}
		else
		{
			// All characters have been displayed, schedule the text clearing
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTriggeredSpeech::ClearText, ClearDelay, false);
		}
	
	}
}

void UTriggeredSpeech::ClearText()
{
	if (TextBlock)
	{
		TextBlock->SetText(FText::GetEmpty());
		CurrentIndex = 0;
	}
}

void UTriggeredSpeech::StartTextGeneration()
{
	CurrentIndex = 0;
	UpdateText();
}