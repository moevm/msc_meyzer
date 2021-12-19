// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ModelActor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssimpBPLibrary.generated.h"

struct aiNode;
struct aiScene;
struct aiMesh;

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

	static void ProcessNode(AModelActor* parent, const aiNode* node, const aiScene* scene);
	static void ProcessMesh(AModelActor* model, const aiMesh* mesh, const aiScene* scene, int32_t section);
};
