// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "CompassShell.generated.h"

UCLASS()
class TOPDOWNRTSTEMPLATE_API ACompassShell : public AActor
{
	GENERATED_BODY()
	
public:
	ACompassShell();

	virtual void Init();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetUnitBase(class AUnitBase* NewUnitBase);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compass")
	UStaticMeshComponent* ShellMesh;

	UPROPERTY(EditAnywhere, Category = "Compass")
	TArray<TSubclassOf<class ACompassPin>> CompassPinClasses;

	UPROPERTY(EditAnywhere, Category = "Compass")
	FRotator ShellRotation = FRotator::ZeroRotator;
	
private:
	UPROPERTY()
	class AUnitBase* UnitBase;

};
