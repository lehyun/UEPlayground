// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"


UCLASS()
class CONSOLEVARIABLE_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = "My")
	static void DebugPrint(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "My")
	static void PrintAllCVars();

	UFUNCTION(BlueprintCallable, Category = "My")
	static void RegisterNewCVar();

	UFUNCTION(BlueprintCallable, Category = "My")
	static void UnregisterNewCVar();

	UFUNCTION(BlueprintCallable, Category = "My")
	static void ModifyNewCVar();

	UFUNCTION(BlueprintCallable, Category = "My")
	static void ApplyScalabilityGroup();

	UFUNCTION(BlueprintCallable, Category = "My")
	static void PrintSettins();



};
