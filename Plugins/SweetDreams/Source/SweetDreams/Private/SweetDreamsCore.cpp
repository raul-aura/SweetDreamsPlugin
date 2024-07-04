// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsCore.h"
#include "Kismet/GameplayStatics.h"

USweetDreamsCore::USweetDreamsCore()
	:
	UserSettings(FDreamUserSettings())
{

}

void USweetDreamsCore::LoadSettings()
{
	CoreSettings = GetMutableDefault<USweetDreamsSettings>();

	// SAVE
	SaveSlotPersistent = CoreSettings->PersistentSlot;
	SaveSlotLocal = CoreSettings->LocalSlot;
	SaveClassPersistent = CoreSettings->PersistentClass;
	SaveClassLocal = CoreSettings->LocalClass;
	if (SaveSlotPersistent == "")
	{
		SaveSlotPersistent = "SweetDream_PERSISTENT";
	}
	if (SaveSlotLocal == "")
	{
		SaveSlotLocal = "SweetDream_LOCAL";
	}

	// GET SUBSYSTEMS
}

void USweetDreamsCore::Initialize(FSubsystemCollectionBase& Collection)
{
	LoadSettings();
	PrintDream(nullptr, "Initializing Sweet Dreams Core subsystem.");
	if (CoreSettings->bEnableAutoCreateSave)
	{
		CreateSave(SaveClassPersistent);
		CreateSave(SaveClassLocal, false);
	}
	if (CoreSettings->bEnableAutoLoadSave)
	{
		LoadSave();
		LoadSave(false);
	}
	Super::Initialize(Collection);
}

void USweetDreamsCore::Deinitialize()
{
	UGameplayStatics::DeleteGameInSlot(SaveSlotLocal, 0);
	Super::Deinitialize();
}

// Debug
void USweetDreamsCore::PrintDream(UObject* DreamOrigin, FString Dream, EPrintType Severity, float Duration)
{
	FString Origin = "[SweetDreams]";
	if (DreamOrigin)
	{
		Origin = FString::Printf(TEXT("[%s]"), *DreamOrigin->GetName());
	}
	Dream = Origin + " " + Dream;
	FColor DreamColor;
	switch (Severity)
	{
	case EPrintType::INFO:
		DreamColor = FColor(195, 150, 255);
		break;
	case EPrintType::WARNING:
		DreamColor = FColor(255, 191, 64);
		break;
	case EPrintType::ERROR:
		DreamColor = FColor(216, 29, 29);
		break;
	}
	UE_LOG(LogActor, Display, TEXT("%s"), *Dream);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, DreamColor, Dream);
	}
}

// Settings
void USweetDreamsCore::SetUserSettings(FDreamUserSettings Settings)
{
	PrintDream(nullptr, "Overriding User Settings.");
	UserSettings = Settings;
}

FDreamUserSettings USweetDreamsCore::GetUserSettings() const
{
	return UserSettings;
}

// SAVE

bool USweetDreamsCore::CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, bool bIsPersistent)
{
	FString SaveSlot = bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
	FString SaveName = bIsPersistent ? "Persistent Save" : "Local Save";
	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, 0))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s ALREADY EXISTS and WILL NOT be created again."), *SaveName));
		return false;
	}
	USaveGame* SaveObject = UGameplayStatics::CreateSaveGameObject(SaveClass);
	if (SaveObject)
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s CREATED with SUCCESS."), *SaveName));
		if (bIsPersistent)
		{
			SavePersistentRef = Cast<USweetDreamsSavePersistent>(SaveObject);
			Save(SavePersistentRef);
		}
		else
		{
			SaveLocalRef = Cast<USweetDreamsSaveLocal>(SaveObject);
			Save(SaveLocalRef, false);
		}
		return true;
	}
	PrintDream(nullptr, FString::Printf(TEXT("%s NOT CREATED. %s reference will be NULL."), *SaveName));
	return false;
}

bool USweetDreamsCore::Save(USweetDreamsSaveFile* SaveObject, bool bIsPersistent)
{
	FString SaveSlot = bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
	FString SaveName = bIsPersistent ? "Persistent Save" : "Local Save";
	if (UGameplayStatics::SaveGameToSlot(SaveObject, SaveSlot, 0))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s SAVED with SUCCESS."), *SaveName));
		ManageSaveData(true, bIsPersistent);
		return true;
	}
	PrintDream(nullptr, FString::Printf(TEXT("%s FAILED to SAVE."), *SaveName));
	return false;
}

USweetDreamsSaveFile* USweetDreamsCore::LoadSave(bool bIsPersistent)
{
	FString SaveSlot = bIsPersistent ? SaveSlotPersistent : SaveSlotLocal;
	FString SaveName = bIsPersistent ? "Persistent Save" : "Local Save";
	if (USaveGame* SaveObject = UGameplayStatics::LoadGameFromSlot(SaveSlot, 0))
	{
		PrintDream(nullptr, FString::Printf(TEXT("%s LOADED and returned with SUCCESS."), *SaveName));
		if (bIsPersistent)
		{
			SavePersistentRef = Cast<USweetDreamsSavePersistent>(SaveObject);
			ManageSaveData(false);
			return SavePersistentRef;
		}
		else
		{
			SaveLocalRef = Cast<USweetDreamsSaveLocal>(SaveObject);
			ManageSaveData(false, false);
			return SaveLocalRef;
		}
	}
	PrintDream(nullptr, FString::Printf(TEXT("%s FAILED to LOAD."), *SaveName));
	return nullptr;
}

void USweetDreamsCore::ManageSaveData(bool bIsSaving, bool bIsPersistent)
{
	USweetDreamsSaveFile* SaveObject;
	if (bIsPersistent)
	{
		SaveObject = SavePersistentRef;
	}
	else
	{
		SaveObject = SaveLocalRef;
	}
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USweetDreamsSaveInterface::StaticClass(), Actors);
	if (&Actors)
	{
		if (bIsSaving)
		{
			SaveObject->OnSaveSaved(Actors);
		}
		else
		{
			SaveObject->OnSaveLoaded(Actors);
		}
	}
}
