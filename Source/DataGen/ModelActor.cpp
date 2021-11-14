// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelActor.h"

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

void AModelActor::SetModel(const UAssimpModel& assimpModel)
{
    for (int32 section = 0; section < assimpModel._sectionCount; ++section)
    {
        // TODO: One ProceduralMeshComponent probably can't have multiple meshes.
        mesh->CreateMeshSection_LinearColor(section, assimpModel._vertices[section], assimpModel._indices[section], assimpModel._normals[section], assimpModel._uvs[section], assimpModel._vertexColors[section], assimpModel._tangents[section], false);
    }


    for (int32 i = 0; i < mesh->GetNumSections(); ++i)
    {
        auto MaterialDyn = UMaterialInstanceDynamic::Create(Material, this);
        mesh->SetMaterial(i, MaterialDyn);
        if (!mesh->GetMaterial(i) || !mesh->IsMeshSectionVisible(i))
        {
            volatile auto msg = "NOT VISIBLE!";
        }
    }

    mesh->CastShadow = true;
    mesh->SetCollisionObjectType(ECC_WorldDynamic);
    mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    mesh->SetCollisionResponseToAllChannels(ECR_Block);
    mesh->UpdateCollisionProfile();
}

