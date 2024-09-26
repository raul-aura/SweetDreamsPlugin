// Copyright Epic Games, Inc. All Rights Reserved.

#include "SweetDreamsBPLibrary.h"
#include "SweetDreams.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "SweetDreamsGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Curves/CurveFloat.h"
#include "Engine/Engine.h"

USweetDreamsCore* USweetDreamsBPLibrary::SweetDreamsCore = nullptr;

USweetDreamsBPLibrary::USweetDreamsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

USweetDreamsCore* USweetDreamsBPLibrary::GetSweetDreamsCore(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
	}
	return SweetDreamsCore;
}

ASweetDreamsGameMode* USweetDreamsBPLibrary::GetSweetDreamsGameMode(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	return Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(World));
}

bool USweetDreamsBPLibrary::CreateSaveGame(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool bIsPersistent)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return false;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			return SweetDreamsCore->CreateSave(SaveClass, bIsPersistent);
		}
	}
	return false;
}

bool USweetDreamsBPLibrary::SaveGame(const UObject* WorldContext, USweetDreamsSaveFile* SaveObject, bool bIsPersistent)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return false;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			return SweetDreamsCore->Save(SaveObject, bIsPersistent);
		}
	}
	return false;
}

USweetDreamsSaveFile* USweetDreamsBPLibrary::LoadSaveGame(const UObject* WorldContext, bool bIsPersistent)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			return SweetDreamsCore->LoadSave(bIsPersistent);
		}
	}
	return nullptr;
}

USweetDreamsSavePersistent* USweetDreamsBPLibrary::GetPersistentSave(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			return SweetDreamsCore->SavePersistentRef;
		}
	}
	return nullptr;
}

USweetDreamsSaveLocal* USweetDreamsBPLibrary::GetLocalSave(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			return SweetDreamsCore->SaveLocalRef;
		}
	}
	return nullptr;
}

FDreamUserSettings USweetDreamsBPLibrary::GetUserSettings(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return FDreamUserSettings();
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			FDreamUserSettings Settings = SweetDreamsCore->GetUserSettings();
			return Settings;
		}
	}
	return FDreamUserSettings();
}

void USweetDreamsBPLibrary::SetUserSettings(const UObject* WorldContext, FDreamUserSettings Settings)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			Settings.ApplySettings();
			SweetDreamsCore->SetUserSettings(Settings);
		}
	}
}

void USweetDreamsBPLibrary::SetSettingsQuality(const UObject* WorldContext, int32 Quality)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return;
	}
	if (UGameInstance* GameInstance = World->GetGameInstance())
	{
		SweetDreamsCore = GameInstance->GetSubsystem<USweetDreamsCore>();
		if (SweetDreamsCore)
		{
			Quality = FMath::Clamp(Quality, 0, 2);
			FDreamUserSettings NewSettings(Quality);
			NewSettings.ApplySettings();
			SweetDreamsCore->SetUserSettings(NewSettings);
		}
	}
}

void USweetDreamsBPLibrary::LoadLevel(const UObject* WorldContext, TSoftObjectPtr<UWorld> Level)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return;
	}
	GetSweetDreamsCore(WorldContext)->LoadLevel(Level);
}

TSoftObjectPtr<UWorld> USweetDreamsBPLibrary::GetCurrentLoadingLevel(const UObject* WorldContext)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	return GetSweetDreamsCore(WorldContext)->CurrentLoadingLevel;
}

float USweetDreamsBPLibrary::GetLoadingPercentage(TSoftObjectPtr<UObject> Asset)
{
	if (Asset.IsNull()) return 100.f;
	const FName AssetName = FName(Asset.GetLongPackageName());
	float Percentage = GetAsyncLoadPercentage(AssetName);
	return (Percentage == -1) ? 100.f : Percentage;
}

bool USweetDreamsBPLibrary::CalculateChance(float& RandomizedValue, float Chance)
{
	Chance = FMath::Clamp(Chance / 100.0f, 0.0f, 1.0f);
	RandomizedValue = FMath::FRandRange(0.0f, 1.0f); 
	return RandomizedValue <= Chance;
}

bool USweetDreamsBPLibrary::CheckInterval(const float& Number, float Interval)
{
	float Reminder = FMath::Fmod(Number, Interval);
	return FMath::IsNearlyZero(Reminder, 0.01f);
}

void USweetDreamsBPLibrary::ShouldNotHappen(const UObject* WorldContext)
{
	PrintDream(WorldContext, "Should NOT happen called.", EPrintType::ERROR, 10.f);
}

void USweetDreamsBPLibrary::DoSomething(const UObject* WorldContext)
{
	PrintDream(WorldContext, "Something has been done.", EPrintType::WARNING, 10.f);
}

float USweetDreamsBPLibrary::IncrementAlpha(const UObject* WorldContext, float& Alpha, float MaxValue, UCurveFloat* AlphaCurve, bool bStopCondition)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return Alpha;
	}
	if (MaxValue <= 0.f)
	{
		bStopCondition = false;
	}
	if (!bStopCondition && Alpha <= MaxValue)
	{
		Alpha += World->GetDeltaSeconds();
		if (AlphaCurve)
		{
			float CurveAlpha = AlphaCurve->GetFloatValue(Alpha);
			return FMath::Clamp(CurveAlpha, 0.0f, MaxValue);
		}
		Alpha = FMath::Clamp(Alpha, 0.0f, MaxValue);
	}
	return Alpha;
}

void USweetDreamsBPLibrary::PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity, float Duration)
{
	UWorld* World = GEngine->GetWorldFromContextObject(DreamOrigin, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(DreamOrigin), TEXT("World Context was not valid.")))
	{
		return;
	}
	GetSweetDreamsCore(DreamOrigin)->PrintDream(DreamOrigin, Dream, Severity, Duration);
}
