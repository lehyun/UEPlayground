// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
#include "MyBlueprintFunctionLibrary.h"



void AMyGameModeBase::DoThing()
{
	UMyBlueprintFunctionLibrary::DebugPrint(TEXT("DoThing() called."));
}
