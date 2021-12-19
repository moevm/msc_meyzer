// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelActor.h"

#include "Materials/Material.h"

// Sets default values
AModelActor::AModelActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    
    if (Material == NULL)
    {
        auto objFinder = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Resources/Material.Material'"));
        check(objFinder.Object != NULL);
        Material = objFinder.Object;
    }

    mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
    RootComponent = mesh;
}

// Called when the game starts or when spawned
void AModelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AModelActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

// Called every frame
void AModelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AModelActor::AddMesh(int32_t section, const TArray<FVector>& vertices, 
    const TArray<int32_t>& indices, const TArray<FVector>& normals, 
    const TArray<FVector2D>& uvs, const TArray<FLinearColor>& vertexColors, 
    const TArray<FProcMeshTangent>& tangents)
{
    mesh->CreateMeshSection_LinearColor(section, vertices, indices, 
        normals, uvs, vertexColors, tangents, true);
    auto MaterialDyn = UMaterialInstanceDynamic::Create(Material, this);
    mesh->SetMaterial(section, MaterialDyn);
}

void AModelActor::SetMaterialParameters(int32_t section, const MaterialParameters& value)
{
    auto material = Cast<UMaterialInstanceDynamic>(mesh->GetMaterial(section));

    material->SetScalarParameterValue("DiffuseSource", static_cast<float>(value.DiffuseType));
    switch (value.DiffuseType)
    {
    case SourceType::Scalar:
        // TODO
        break;
    case SourceType::Vertex:
        // TODO
        break;
    case SourceType::Texture:
        material->SetTextureParameterValue("DiffuseTexture", value.DiffuseTexture);
        break;
    default:
        break;
    }

    params = value; // TODO: Allow editing in editor?
}

void AModelActor::FinishConstruction()
{
    mesh->CastShadow = true;
    mesh->SetCollisionObjectType(ECC_WorldDynamic);
    mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    mesh->SetCollisionResponseToAllChannels(ECR_Block);
    //mesh->UpdateCollisionProfile();
}
