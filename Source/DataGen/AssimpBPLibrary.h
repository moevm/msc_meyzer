// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssimpModel.h"
#include "ModelActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssimpBPLibrary.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DATAGEN_API UAssimpBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Assimp")
	static AModelActor* LoadModel(const FString& actorName, const FString& modelPath, const FTransform& transform, AActor* parent, FString& ErrorCode);

private:

	static void ProcessMesh(UAssimpModel* model, aiMesh* mesh, const aiScene* scene);
	static void ProcessNode(UAssimpModel* model, aiNode* node, const aiScene* scene, const FTransform& parentTransform = FTransform::Identity);
};
