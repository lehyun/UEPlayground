// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "MyDeveloperSettings.h"
#include "MyCustomSettings.h"

// ECVF_ReadOnly - 옵션. 읽기 전. 콘솔에서 수정 불가.
// ECVF_Cheat - 옵션. 최종 릴리즈 빌드에서만 ReadOnly와 동일.
// ECVF_Unregistered - 옵션이 아니라 상태. UnregisterconsoleObject 호출시, bKeepState가 true일때, 변수는 살아있지만, 읽기/쓰기 불가 상태를 나타내는 플래그.
// ECVF_CreatedFromIni - 옵션이 아니라 상태. INI파일로 부터 최초 생성된 콘솔 변수의 경우, 이 플래그가 붙는다.
// ECVF_RenderThreadSafe - 옵션. 랜더쓰레드에 복사본을 두고 렌더 쓰레드 명령을 통해 동기화 한다. 다른쓰레드로 확장가능. 콘솔변수참조는 렌더쓰레드에서만 사용된다고 가정한다. 다른쓰레드에서 변수참조는 하지 마세요. 동적 변수등록과 함께 사용될 수 없다.
// ECVF_Scalability - 옵션. 설정되어있지 않다면, ApplyCVarSettingsGroupFromIni에서 경고 발생. ECVF_Cheat와 함께 사용되지 말아야함.
// ECVF_ScalabilityGroup - ECVF_Scalability속성을 가진 변수들을 제어한다. 이름은 항상 sg.로 시작해야한다.

static TAutoConsoleVariable<int32> CVarHyunLeeExample1(
	TEXT("hl.ExampleVariable1"),
	1,
	TEXT("This is an example variable of TAutoConsoleVariable<int32>.\n")
	TEXT("Default Value = 1\n"),
	ECVF_Scalability | ECVF_RenderThreadSafe);


// 기존의 static 변수와 맵핑하는 법 (int32, float만 가능)
// -- 생성자 참고
static float HLExampleVariable2 = 2.0f;
static FAutoConsoleVariableRef CVarHyunLeeExample2(
	TEXT("hl.ExampleVariable2"),
	HLExampleVariable2,
	TEXT("This is ExampleVariable of FAutoConsoleVariableRef\n")
	TEXT("static float HLExampleVariable2 = 2.0f;\n"),
	ECVF_Cheat);

// 이현 스케일 그룹 설정
// DefaultScalability.ini에서 그룹설정 가능
static TAutoConsoleVariable<int32> CVarHyunLeeGroup(
	TEXT("sg.HyunLeeGroup"),
	3,
	TEXT("이현 그룹\n")
	TEXT(" 0:low, 1:med, 2:high, default: 3"),
	ECVF_ScalabilityGroup);



void UMyBlueprintFunctionLibrary::DebugPrint(const FString& Text)
{
	UKismetSystemLibrary::PrintString(GEngine->GetWorld(), Text);
}

void UMyBlueprintFunctionLibrary::PrintAllCVars()
{
	{
		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("hl.ExampleVariable1"));
		FString Text = FString::Printf(TEXT("hl.ExampleVariable1 = %d (0x%08X)"), CVar->GetInt(), (int)CVar->GetFlags());
		DebugPrint(Text);

		checkCode(ensure(CVar->GetInt() == CVarHyunLeeExample1.GetValueOnGameThread()));
	}
	{
		static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("hl.ExampleVariable2"));
		FString Text = FString::Printf(TEXT("hl.ExampleVariable2 = %f (0x%08X)"), CVar->GetFloat(), (int)CVar->GetFlags());
		DebugPrint(Text);
	}
	{
		const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("hl.ExampleVariable3"));
		if (CVar != nullptr)
		{
			FString Text = FString::Printf(TEXT("hl.ExampleVariable3 = %s (0x%08X)"), *CVar->GetString(), (int)CVar->GetFlags());
			DebugPrint(Text);
		}
	}
}

void UMyBlueprintFunctionLibrary::RegisterNewCVar()
{
	// ECVF_RenderThreadSafe 옵션 사용 불가. Assert에 걸린다
	//uint32 Flags = ECVF_Scalability | ECVF_RenderThreadSafe;

	// ECVF_Unregistered 옵션이 아니고 상태이다.
	// UnregisterconsoleObject 호출시, bKeepState가 true일때, 변수는 살아있지만, 읽기/쓰기 불가 상태를 나타내는 플래그.
	//uint32 Flags = ECVF_Unregistered;

	uint32 Flags = ECVF_Cheat;

	// 동적으로 콘솔 변수 등록하기
	IConsoleManager::Get().RegisterConsoleVariable(
		TEXT("hl.ExampleVariable3"),
		TEXT("3(string)"),
		TEXT("This is ExampleVariable of dynamic registration\n")
		TEXT("Default Value = 3\n"),
		Flags);
}

void UMyBlueprintFunctionLibrary::UnregisterNewCVar()
{
	const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("hl.ExampleVariable3"));
	if (CVar != nullptr) IConsoleManager::Get().UnregisterConsoleObject(CVar, false);
}

void UMyBlueprintFunctionLibrary::ModifyNewCVar()
{
	static int32 Index = 0;
	const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("hl.ExampleVariable3"));
	if (CVar != nullptr) CVar->Set(*FString::Printf(TEXT("modified(string) %d"), ++Index));
}

// 보통은 ScalabilityGroup의 변경을 감지하여, 그때 마다 호출한다.
void UMyBlueprintFunctionLibrary::ApplyScalabilityGroup()
{
	int32 Value = CVarHyunLeeGroup.GetValueOnGameThread();
	ApplyCVarSettingsGroupFromIni(TEXT("HyunLeeGroup"), Value, *GScalabilityIni, ECVF_SetByScalability);
}

void UMyBlueprintFunctionLibrary::PrintSettings()
{
	UMyDeveloperSettings* MyDeveloperSettings = GetMutableDefault<UMyDeveloperSettings>();
	if (MyDeveloperSettings)
	{
		DebugPrint(FString::Printf(TEXT("MyDeveloperSettings.bSampleBool = %s"), MyDeveloperSettings->bSampleBool ? TEXT("True") : TEXT("False")));

		for (const auto& Str : MyDeveloperSettings->SampleStringArray)
		{
			DebugPrint(FString::Printf(TEXT("MyDeveloperSettings.SampleStringArray = %s"), *Str));
		}
	}

	UMyCustomSettings* MyCustomSettings = GetMutableDefault<UMyCustomSettings>();
	if (MyCustomSettings)
	{
		DebugPrint(FString::Printf(TEXT("MyCustomSettings.bSampleBool = %s"), MyCustomSettings->bSampleBool ? TEXT("True") : TEXT("False")));
		DebugPrint(FString::Printf(TEXT("MyCustomSettings.SampleFloatRequireRestart = %f"), MyCustomSettings->SampleFloatRequireRestart));
	}
}

void UMyBlueprintFunctionLibrary::TestArray(int32 Size)
{
	FTestParent Parent1;
	int32 MaxNum = Size;
	for (int32 Index = 0; Index < MaxNum; ++Index)
	{
		FTestChild Child;
		Child.DebugName = FString::Printf(TEXT("자식노드_%d"), Index);
		Parent1.Children.Add(Child);
	}

	for (int32 Index = 0; Index < MaxNum; ++Index)
	{
		FTestParent Parent2;
		Parent2.Children = Parent1.Children;

		MaxNum = Parent2.Children.Num();
		for (int32 Index = MaxNum - 1; Index >= 0; --Index)
		{
			FTestChild Child = Parent2.Children[Index];
			Parent2.Children.Add(Child);
			Parent2.Children.RemoveAt(Index);
		}

		for (auto Child : Parent2.Children)
		{
			DebugPrint(Child.DebugName);
		}
	}
}

void UMyBlueprintFunctionLibrary::TestAny()
{

}

#include "ProceduralMeshcomponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"

#define PRINT_INT(x) UMyBlueprintFunctionLibrary::DebugPrint(FString::Printf(TEXT("%s = %d"), TEXT(#x), x));

AImposterSpriteGenerator::AImposterSpriteGenerator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));

	ProceduralMeshComponent->SetupAttachment(DummyRoot);
	SceneCaptureComponent2D->SetupAttachment(DummyRoot);
	RootComponent = DummyRoot;
}

void AImposterSpriteGenerator::SetupRTAndSaveList(int InRes, int InSceneCaptureRes)
{
	float ResMips = FMath::Clamp((int32)FMath::FloorLog2((float)InRes), 9, 14);
	Resolution = FMath::TruncToInt(FMath::Pow(ResMips, 2));
	PRINT_INT(Resolution);

	SceneCaptureMips = FMath::Clamp((int32)FMath::FloorLog2((float)InSceneCaptureRes), 7, 11);
	PRINT_INT(SceneCaptureMips);

	CutoutMipTarget = SceneCaptureMips - 4;
	PRINT_INT(CutoutMipTarget);

	DFMipTarget = CutoutMipTarget + 3;
	PRINT_INT(DFMipTarget);

	SceneCatpureResolution = FMath::TruncToInt(FMath::Pow(SceneCaptureMips, 2));
	PRINT_INT(SceneCatpureResolution);

	// Allocate Targets for User Maps when Necessary
	// Create RTs
	for (const ELMImposterBakeType& BakeType : MapsToRender)
	{
		UTextureRenderTarget2D* RT = nullptr;
		UTextureRenderTarget2D** RTAddr = TargetMaps.Find(BakeType);
		if (RTAddr) RT = *RTAddr;
		if (!RT->IsValidLowLevel() || RT->SizeX != Resolution)
		{
			RT = UKismetRenderingLibrary::CreateRenderTarget2D(this, Resolution, Resolution, RTF_RGBA16f);
			*RTAddr = RT;
		}
	}
	
	// Clear Invalid RTs
	{
		TArray<ELMImposterBakeType> Keys;
		if (TargetMaps.GetKeys(Keys))
		{
			for (const ELMImposterBakeType& BakeType : Keys)
			{
				UTextureRenderTarget2D* RT = nullptr;
				UTextureRenderTarget2D** RTAddr = TargetMaps.Find(BakeType);
				if (RTAddr) RT = *RTAddr;
				if (!RT->IsValidLowLevel())
				{
					TargetMaps.Remove(BakeType);
				}
			}
		}
	}

	// Scene Capture Render Target with Mips
	int32 NumOfMipChain = SceneCaptureMipChain.Num();
	SceneCaptureMipChain.Reserve(8);
	for (int32 MipIdx = 0; MipIdx <= 7; ++MipIdx)
	{
		UTextureRenderTarget2D* CaptureRT = nullptr;
		int32 ResSize = SceneCatpureResolution / FMath::TruncToInt(FMath::Pow(2.0, (float)MipIdx));

		if (MipIdx < NumOfMipChain)
		{
			CaptureRT = SceneCaptureMipChain[MipIdx];
			if (CaptureRT)
			{
				if (CaptureRT->SizeX != ResSize)
				{
					CaptureRT = nullptr;
				}
			}	
		}
		else
		{
			SceneCaptureMipChain.Add(nullptr);
		}
		if (!CaptureRT)
		{
			CaptureRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, ResSize, ResSize, RTF_RGBA16f);
			SceneCaptureMipChain[MipIdx] = CaptureRT;
		}
	}
}
