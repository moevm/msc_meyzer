// Fill out your copyright notice in the Description page of Project Settings.

#include "FeatureTestingActorComponent.h"
#include "assimp/vector2.h"
#include "assimp/vector3.h"
#include "tinyfiledialogs.h"

// #include "assimp"
// Sets default values for this component's properties
UFeatureTestingActorComponent::UFeatureTestingActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFeatureTestingActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	/* const char* filters[1] = { "*.txt" };
	aiVector2D test{ 1.3, 4.1 };
	auto path1 = tinyfd_openFileDialog("Text file", 0, 1, filters, 0, 0);
	if (path1) {
		UE_LOG(LogTemp, Warning, TEXT("Opened text file:"));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(path1));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Action was cancelled"));
	}
	*/


}	


// Called every frame
void UFeatureTestingActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

