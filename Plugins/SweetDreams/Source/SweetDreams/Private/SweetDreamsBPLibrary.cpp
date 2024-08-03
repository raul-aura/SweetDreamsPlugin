// Copyright Epic Games, Inc. All Rights Reserved.

#include "SweetDreamsBPLibrary.h"
#include "SweetDreams.h"
#include "SweetDreamsGameMode.h"
#include "Kismet/GameplayStatics.h"
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

void USweetDreamsBPLibrary::PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity, float Duration)
{
	UWorld* World = GEngine->GetWorldFromContextObject(DreamOrigin, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(DreamOrigin), TEXT("World Context was not valid.")))
	{
		return;
	}
	GetSweetDreamsCore(DreamOrigin)->PrintDream(DreamOrigin, Dream, Severity, Duration);
}
