// Fill out your copyright notice in the Description page of Project Settings.

#include "AssimpBPLibrary.h"
#include "Logging/LogMacros.h"
#include "Kismet/GameplayStatics.h"

namespace 
{
    FTransform getFTransform(const aiMatrix4x4& matrix)
    {
        FMatrix fMatrix;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                fMatrix.M[i][j] = matrix[j][i];
            }
        }
        
        FTransform result{ fMatrix };
        
        return result;
    }
}

AModelActor* UAssimpBPLibrary::LoadModel(const FString& actorName, const FString& modelPath, const FTransform& transform, AActor* parent, FString& ErrorCode)
{
    Assimp::Importer importer;
    // TODO: Add map and check if loaded already
    std::string filename(TCHAR_TO_UTF8(*modelPath));
    const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene)
    {
        ErrorCode = importer.GetErrorString();
        return nullptr;
    }

    auto* model = NewObject<UAssimpModel>();
    model->_sectionCount = 0;
    ProcessNode(model, scene->mRootNode, scene);

    auto deferredActor = Cast<AModelActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(parent, AModelActor::StaticClass(), FTransform::Identity));
    if (deferredActor != nullptr)
    {
        deferredActor->SetModel(*model);
        UGameplayStatics::FinishSpawningActor(deferredActor, FTransform::Identity);
    }

    return deferredActor;
}

void UAssimpBPLibrary::ProcessNode(UAssimpModel* model, aiNode* node, const aiScene* scene, const FTransform& parentTransform /*= FTransform::Identity*/)
{
    const FTransform currentTransform = getFTransform(node->mTransformation);

    for (uint32 i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(model, mesh, scene);
        model->_transforms.Add(currentTransform);
        ++model->_sectionCount;
    }

    // do the same for all of its children
    for (uint32 i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(model, node->mChildren[i], scene, currentTransform);
    }
}

void UAssimpBPLibrary::ProcessMesh(UAssimpModel* model, aiMesh* mesh, const aiScene* scene)
{
    // the very first time this method runs, we'll need to create the empty arrays
    // we can't really do that in the class constructor because we don't know how many meshes we'll read, and this data can change between imports
    if (model->_vertices.Num() <= model->_sectionCount) {
        model->_vertices.AddZeroed();
        model->_normals.AddZeroed();
        model->_uvs.AddZeroed();
        model->_tangents.AddZeroed();
        model->_vertexColors.AddZeroed();
        model->_indices.AddZeroed();
    }

    // we reinitialize the arrays for the new data we're reading
    model->_vertices[model->_sectionCount].Empty();
    model->_normals[model->_sectionCount].Empty();
    model->_uvs[model->_sectionCount].Empty();
    model->_tangents[model->_sectionCount].Empty();
    model->_vertexColors[model->_sectionCount].Empty();
    model->_indices[model->_sectionCount].Empty();

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        FVector vertex, normal;
        // process vertex positions, normals and UVs
        vertex.X = mesh->mVertices[i].x;
        vertex.Y = mesh->mVertices[i].y;
        vertex.Z = mesh->mVertices[i].z;

        normal.X = mesh->mNormals[i].x;
        normal.Y = mesh->mNormals[i].y;
        normal.Z = mesh->mNormals[i].z;

        // if the mesh contains tex coords
        if (mesh->mTextureCoords[0]) {
            FVector2D uvs;
            uvs.X = mesh->mTextureCoords[0][i].x;
            uvs.Y = mesh->mTextureCoords[0][i].y;
            model->_uvs[model->_sectionCount].Add(uvs);
        }
        else {
            model->_uvs[model->_sectionCount].Add(FVector2D(0.f, 0.f));
        }
        model->_vertices[model->_sectionCount].Add(vertex);
        model->_normals[model->_sectionCount].Add(normal);
    }

    // process indices
    for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        model->_indices[model->_sectionCount].Add(face.mIndices[2]);
        model->_indices[model->_sectionCount].Add(face.mIndices[1]);
        model->_indices[model->_sectionCount].Add(face.mIndices[0]);
    }
}
