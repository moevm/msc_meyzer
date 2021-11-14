// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Materials/Material.h"
#include "AssimpModel.h"
#include "ModelActor.generated.h"

UCLASS()
class DATAGEN_API AModelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModelActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetModel(const UAssimpModel& assimpModel);

private:
	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* mesh;

	UMaterial* Material;
};
