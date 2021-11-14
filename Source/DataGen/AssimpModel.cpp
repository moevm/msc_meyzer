// Fill out your copyright notice in the Description page of Project Settings.


#include "AssimpModel.h"

void UAssimpModel::clear()
{
    _vertices.Empty();
    _indices.Empty();
    _normals.Empty();
    _uvs.Empty();
    _tangents.Empty();
    _vertexColors.Empty();
    _sectionCount = 0;
}

bool UAssimpModel::getSection(int32 index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FProcMeshTangent>& Tangents)
{
    if (index >= _sectionCount)
    {
        return false;
    }
    Vertices = _vertices[index];
    Faces = _indices[index];
    Normals = _normals[index];
    UV = _uvs[index];
    Tangents = _tangents[index];
    return true;
}
