// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Actors/Component.h"
#include "Actors/Waypoint.h"
#include "Characters/Unit/UnitBase.h"
#include "Engine/World.h"
#include "ComponentGeneratorGameModeBase.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API AComponentGeneratorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AComponentGeneratorGameModeBase();
	void EndGame();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	FVector CalcLocation(FVector Offset, FVector MinRange, FVector MaxRange);
	FRotator FRandomRotator(FRotator Min, FRotator Max);
	FRotator GetRotationRange(FString Name);
	FRotator GetAbsoluteRotationOffset(FString Name);
	FVector  GetComponentScale(FString Name);
	class UStaticMesh* GetComponentMesh(FString Name);
	FVector  GetComponentRandRangeMin(FString Name);
	FVector  GetComponentRandRangeMax(FString Name);
	FVector  GetComponentAbsoluteOffset(FString Name);
	bool  GetComponentIsMoving(FString Name);
	FVector  GetComponentMovementSpeed(FString Name);
	float  GetComponentMovementRange(FString Name);
	UMaterial* GetComponentMaterial(FString Name);
	int  GetComponentFrequency(FString Name);
	ESpawnActorCollisionHandlingMethod GetCollisionMethod(FString Name);
	TArray<FString> GetComponentNames();
	TArray<FString> ComponentNames;

	class AComponentController* ComponentController;

protected:

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "ComponentClass", Keywords = "TopDownRTSTemplate ComponentClass"), Category = TopDownRTSTemplate)
	TSubclassOf<class AComponent> ComponentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class UDataTable* ComponentDataTable;
	
	FComponentData* ComponentData;
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = TopDownRTSTemplate)
	TArray<class AComponent*> Components;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ComponentSpawn", Keywords = "TopDownRTSTemplate ComponentSpawn"), Category = TopDownRTSTemplate)
	void ComponentSpawn();
	
};
