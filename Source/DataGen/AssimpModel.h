// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "AssimpModel.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DATAGEN_API UAssimpModel : public UObject
{
public:
    GENERATED_BODY()

    int32 _sectionCount;
    TArray<TArray<FVector>> _vertices;
    TArray<TArray<int32>> _indices;
    TArray<TArray<FVector>> _normals;
    TArray<TArray<FVector2D>> _uvs;
    TArray<TArray<FProcMeshTangent>> _tangents;
    TArray<TArray<FLinearColor>> _vertexColors;
    TArray<FTransform> _transforms;

    UFUNCTION(BlueprintCallable, Category = "Assimp")
    bool getSection(int32 index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FProcMeshTangent>& Tangents);

    UFUNCTION(BlueprintCallable, Category = "Assimp")
    void clear();
};
