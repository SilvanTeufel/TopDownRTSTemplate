// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Component.generated.h"


USTRUCT(BlueprintType)
struct FComponentData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	class UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector RandomRangeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector RandomRangeMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	int Frequency;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector AbsoluteOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	bool IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FVector MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	float MovementRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	ESpawnActorCollisionHandlingMethod CollisionMethod;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FRotator RotationRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TopDownRTSTemplate)
	FRotator AbsoluteRotationOffset;
};

UCLASS()
class TOPDOWNRTSTEMPLATE_API AComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FString ComponentName;
	
	void Init(FString Name, class UStaticMesh* Mesh, UMaterial* Material, bool BIsMoving, float FMovementRange, FVector VMovementSpeed); //, class UStaticMeshComponent* MeshComp);

	float CurrentMovementTime = 0.f;

	float CurrentMovementRange = 0.f;

	bool DirectionChange = false;

	bool IsMoving = false;
	
	FVector MovementSpeed = FVector(0.f,0.f,0.f);
	
	float MovementRange = 0.f;

public:	
   class UStaticMeshComponent* ComponentMesh;

};
