// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MyDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEVARIABLE_API UMyDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sample1")
	bool bSampleBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sample2")
	TArray<FString> SampleStringArray;

	
};
