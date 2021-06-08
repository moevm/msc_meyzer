// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileDialogsBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DATAGEN_API UFileDialogsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	/** */
	UFUNCTION(BlueprintCallable)
	static FString openFileDialog(const TArray<FString>& filterPatterns, const FString& filterDescription = "");
};
