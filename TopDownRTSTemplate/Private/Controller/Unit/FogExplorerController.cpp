// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Unit/FogExplorerController.h"
#include "Actors/FogOfWar.h"
#include "Perception/AIPerceptionComponent.h"

AFogExplorerController::AFogExplorerController()
{
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AFogExplorerController::OnDetectedFog);
}

void AFogExplorerController::OnDetectedFog(const TArray<AActor*>& DetectedFog)
{
	for(int j = 0; j < DetectedFog.Num(); j++ )
	{
		AFogOfWar* Fog = Cast<AFogOfWar>(DetectedFog[j]);
		if(Fog)
		{
			for(int i = 0; i < Fog->ExplorersGetHidden.Num(); i++)
			{
				if( Fog->ExplorersGetHidden[i])
				{
					 Fog->ExplorersGetHidden[i]->SetActorHiddenInGame(false);
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT(" Disable FOG!!"));
			//SetFogVisibility(false);
			 Fog->Visible = false;
			 Fog->DisableFog();
		}
	}
}

