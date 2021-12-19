// Fill out your copyright notice in the Description page of Project Settings.

#include "AssimpBPLibrary.h"
#include "Logging/LogMacros.h"
#include "Kismet/GameplayStatics.h"
#include "RenderUtils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    
    UTexture2D* loadTextureFromFile(UObject* outer, const FString& path, const FName inName = NAME_None)
    {
        //TArray<uint8_t> pixelData;
        //pixelData.Empty();
        //return loadTextureFromMemory(outer, pixelData, sizeX, sizeY, format, inName);
    }

    UTexture2D* loadTextureFromMemory(UObject* outer, const uint8_t* pixelData, int32 inSizeX, int32 inSizeY, EPixelFormat inFormat = PF_R8G8B8A8, const FName inName = NAME_None)
    {
        UTexture2D* texture = UTexture2D::CreateTransient(inSizeX, inSizeY, inFormat, inName);
        void* TextureData = texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
        FMemory::Memcpy(TextureData, pixelData, 4 * inSizeX * inSizeY);
        texture->PlatformData->Mips[0].BulkData.Unlock();
        texture->UpdateResource();
        return texture;
    }
}

AModelActor* UAssimpBPLibrary::LoadModel(const FString& actorName, const FString& modelPath, const FTransform& transform, AActor* parent, FString& ErrorCode)
{
    // TODO: Add map and check if loaded already

    Assimp::Importer importer;
    std::string filename(TCHAR_TO_UTF8(*modelPath));
    const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene)
    {
        ErrorCode = importer.GetErrorString();
        return nullptr;
    }

    auto modelActor = NewObject<AModelActor>(parent, AModelActor::StaticClass());
    ProcessNode(modelActor, scene->mRootNode, scene);

    return modelActor;
}

void UAssimpBPLibrary::ProcessNode(AModelActor* parent, const aiNode* node, const aiScene* scene)
{
    const FTransform currentTransform = getFTransform(node->mTransformation);

    AModelActor* subModel = nullptr;
    if (scene->mRootNode != node)
    {
        // Create Model with specified transform, put it in parent one
        subModel = Cast<AModelActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
            parent, AModelActor::StaticClass(), currentTransform));
        if (subModel != nullptr)
        {
            for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
                const aiMesh* const mesh = scene->mMeshes[node->mMeshes[i]];
                ProcessMesh(subModel, mesh, scene, i);
            }
            subModel->FinishConstruction();
            UGameplayStatics::FinishSpawningActor(subModel, currentTransform);
        }

        subModel->AttachToActor(parent, FAttachmentTransformRules::KeepRelativeTransform);
    }
    else
    {
        subModel = parent;
    }
    
    // do the same for all of its children
    for (uint32 i = 0; i < node->mNumChildren; ++i) {
        // Create child models
        ProcessNode(subModel, node->mChildren[i], scene);
    }
}

void UAssimpBPLibrary::ProcessMesh(AModelActor* model, const aiMesh* mesh, 
    const aiScene* scene, int32_t section)
{
    MaterialParameters materialParams;

    TArray<FVector> vertices;
    TArray<int32> indices;
    TArray<FVector> normals;
    TArray<FVector2D> uvs;
    TArray<FProcMeshTangent> tangents;
    TArray<FLinearColor> vertexColors;

    vertices.Empty();
    normals.Empty();
    uvs.Empty();
    tangents.Empty();
    vertexColors.Empty();
    indices.Empty();

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        vertices.Emplace(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );
        normals.Emplace(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        if (mesh->HasTextureCoords(0))
        {
            uvs.Emplace(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            uvs.Emplace(0.f, 0.f);
        }

        if (mesh->HasVertexColors(0))
        {
            const auto& color = mesh->mColors[0][i];
            vertexColors.Emplace(color.r, color.g, color.b, color.a);
            materialParams.DiffuseType = SourceType::Vertex;
        }
    }
    // process indices
    for (uint32 i = 0; i < mesh->mNumFaces; ++i) 
    {
        const aiFace& face = mesh->mFaces[i];
        indices.Add(face.mIndices[2]);
        indices.Add(face.mIndices[1]);
        indices.Add(face.mIndices[0]);
    }

    model->AddMesh(section, vertices, indices, normals, uvs, vertexColors, tangents);

    auto meshMaterial = scene->mMaterials[mesh->mMaterialIndex];

    aiString path;
    if (materialParams.DiffuseType != SourceType::Vertex &&
        aiReturn_SUCCESS == meshMaterial->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path))
    {
        // TODO: Make texture loading as separate function
        if (path.data[0] == '*') // Texture is embedded
        {
            const FString texturePath{ path.C_Str() + 1 }; // Skip asterix
            const auto idx = FCString::Atoi(*texturePath);
            const aiTexture* aTexture = scene->mTextures[idx];
            auto loadedData = reinterpret_cast<uint8_t*>(aTexture->pcData);
            auto width = aTexture->mWidth;
            auto height = aTexture->mHeight;

            if (height == 0)
            {
                // Texture is compressed, decompress it
                int w, h, n;
                loadedData = stbi_load_from_memory(loadedData, aTexture->mWidth, &w, &h, &n, 4);
                checkf(n == 4, TEXT("Only images with 4 channels are supported at the moment"));
                width = w;
                height = h;
            }

            materialParams.DiffuseType = SourceType::Texture;
            materialParams.DiffuseTexture = loadTextureFromMemory(model, loadedData, width, height);
            checkf(materialParams.DiffuseTexture, TEXT("Could not load texture for some reason"));
            // TODO: Check memory leaks with aiTexture and loaded image from stb
        }

        // TODO: Get full path of the texture
        //FString folderPath = FPaths::GetPath(meshPath);
        //UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(path.C_Str()));
        //materialParams.DiffuseType = SourceType::Texture;
        //materialParams.DiffuseTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *texturePath));

    }

    // TODO: Check remaining textures:
    // Metallic
    // Specular
    // Roughness

    // Prompt to enter value for use if texture is not found?

    model->SetMaterialParameters(section, materialParams);
}
