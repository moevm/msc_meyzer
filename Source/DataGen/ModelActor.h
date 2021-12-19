// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ModelActor.generated.h"

class UMaterial;


enum class SourceType
{
    Scalar,
    Vertex,
    Texture,
    Count,
};

struct MaterialParameters
{
    SourceType DiffuseType = SourceType::Scalar;
    UTexture2D* DiffuseTexture = nullptr;
	FVector DiffuseColor = { 1.0, 0.0, 1.0 };

    SourceType MetallicType = SourceType::Scalar;
    UTexture2D* MetallicTexture = nullptr;
    FFloat32 MetallicValue = 0.5f;

    SourceType SpecularType = SourceType::Scalar;
    UTexture2D* SpecularTexture = nullptr;
    FFloat32 SpecularValue = 0.5f;

    SourceType RoughnessType = SourceType::Scalar;
    UTexture2D* RoughnessTexture = nullptr;
    FFloat32 RoughnessValue = 0.5f;
};

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
	
	void AddMesh(int32_t section, const TArray<FVector>& vertices,
		const TArray<int32_t>& indices, const TArray<FVector>& normals,
		const TArray<FVector2D>& uvs, const TArray<FLinearColor>& vertexColors,
		const TArray<FProcMeshTangent>& tangents);
	void SetMaterialParameters(int32_t section, const MaterialParameters& value);
	void FinishConstruction();

private:
	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* mesh;
	UPROPERTY(EditAnywhere)
	UMaterial* Material;

	MaterialParameters params;
};
