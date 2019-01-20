// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"


USTRUCT()
struct FTestChild
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, NonTransactional)
	FString DebugName;
};

USTRUCT()
struct FTestParent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, NonTransactional)
	FString DebugName;

	UPROPERTY(VisibleAnywhere, NonTransactional)
	TArray<FTestChild> Children;
};

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
	static void PrintSettings();

	UFUNCTION(BlueprintCallable, Category = "My")
	static void TestArray(int32 Size);

	UFUNCTION(BlueprintCallable, Category = "My")
	static void TestAny();
};

UENUM()
enum class ELMImposterLayoutType
{
	FullSphereView,
	UpperHemisphereOnly,
	TraditionalBillboards
};

UENUM()
enum class ELMImposterBakeType
{
	BaseColor,
	Metallic,
	Specular,
	Roughness,
	Emissive,
	Opacity,
	WorldNormal,
	Subsurface,
	Depth,
	Lit,
	CustomLighting
};

UCLASS()
class AImposterSpriteGenerator : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY()
	class USceneComponent* DummyRoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UProceduralMeshComponent* ProceduralMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneCaptureComponent2D* SceneCaptureComponent2D;
	
public:
	UPROPERTY(Category = Imposter, EditAnywhere, BlueprintReadWrite)
	int32 FramesXY;
	UPROPERTY(Category = Imposter, EditAnywhere, BlueprintReadWrite)
	uint32 bUseDistanceFieldAlpha : 1;
	UPROPERTY(Category = Imposter, EditAnywhere, BlueprintReadWrite)
	FString NewAssetPath;
	UPROPERTY(Category = Imposter, EditAnywhere, BlueprintReadWrite)
	FString NewAssetTexturePrefix;
	UPROPERTY(Category = Imposter, EditAnywhere, BlueprintReadWrite)
	FString NewAssetMICName;
	UPROPERTY(Category = Imposter, EditAnywhere, BlueprintReadWrite)
	TArray<ELMImposterBakeType> MapsToRender;
	UPROPERTY(Category = Imposter, EditAnywhere, BlueprintReadWrite)
	ELMImposterLayoutType ImposterType;

public:
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	uint32 bCustomLightingAndBaseColor : 1;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	uint32 bCombineNormalAndDepth : 1;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	uint32 bUseMeshCutout : 1;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	float CameraDistance;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	int32 Resolution;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	int32 SceneCatpureResolution;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	int32 SceneCaptureMips;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	int32 CutoutMipTarget;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	int32 CheckTargetMipSize;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	int32 DFMipTarget;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	float BillboardTopOffset;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	float BillboardTopOffsetCenter;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	TArray<FVector> ManualPointArray;
	UPROPERTY(Category = Advanced, EditAnywhere, BlueprintReadWrite)
	uint32 bShowCutoutDebug : 1;

public:
	UPROPERTY(Category = RenderTargets, EditAnywhere, BlueprintReadWrite)
	TMap<ELMImposterBakeType, UTextureRenderTarget2D*> TargetMaps;
	UPROPERTY(Category = RenderTargets, EditAnywhere, BlueprintReadWrite)
	TArray<UTextureRenderTarget2D*> SceneCaptureMipChain;
	UPROPERTY(Category = RenderTargets, EditAnywhere, BlueprintReadWrite)
	class UTextureRenderTarget2D* CombinedAlphaRT;
	UPROPERTY(Category = RenderTargets, EditAnywhere, BlueprintReadWrite)
	class UTextureRenderTarget2D* ScratchRT;

public:
	UFUNCTION(Category = Imposter, BlueprintCallable)
	void SetupRTAndSaveList(int InRes, int InSceneCaptureRes);
};