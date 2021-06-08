// Fill out your copyright notice in the Description page of Project Settings.


#include "FileDialogsBPLibrary.h"
#include "tinyfiledialogs.h"

FString UFileDialogsBPLibrary::openFileDialog(const TArray<FString>& filterPatterns, const FString& filterDescription) {
	TArray<char*> filters;
	for (const auto& filter : filterPatterns) {
		filters.Add(TCHAR_TO_ANSI(*filter));
	}
	auto openedFilePath = tinyfd_openFileDialog("Text file", 0, 1, &filters[0], TCHAR_TO_ANSI(*filterDescription), 0);

	return FString(openedFilePath);
}
