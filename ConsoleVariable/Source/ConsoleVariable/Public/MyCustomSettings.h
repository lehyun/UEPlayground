// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyCustomSettings.generated.h"

/**
* Setting object used to hold both config settings and editable ones in one place
* To ensure the settings are saved to the specified config file make sure to add
* props using the globalconfig or config meta.
*/
UCLASS(config = Game, defaultconfig)
class UMyCustomSettings : public UObject
{
	GENERATED_BODY()

public:
	UMyCustomSettings(const FObjectInitializer& ObjectInitializer);

	/** Sample bool property */
	UPROPERTY(EditAnywhere, config, Category = Custom)
	bool bSampleBool;

	/** Sample float property that requires a restart */
	UPROPERTY(EditAnywhere, config, Category = Custom, meta = (ConfigRestartRequired = true))
	float SampleFloatRequireRestart;

	/** Sample string list */
	UPROPERTY(config, EditAnywhere, Category = Custom)
	TArray<FString> SampleStringList;

	/** Or add min, max or clamp values to the settings */
	UPROPERTY(config, EditAnywhere, Category = Custom, meta = (UIMin = 1, ClampMin = 1))
	int32 ClampedIntSetting;

	/** We can even use asset references */
	UPROPERTY(config, EditAnywhere, Category = Materials, meta = (AllowedClasses = "MaterialInterface"))
	FStringAssetReference StringMaterialAssetReference;

};
