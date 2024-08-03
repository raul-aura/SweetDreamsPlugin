// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsSavePersistent.h"
#include "SweetDreamsSaveLocal.h"
#include "SweetDreamsSettings.h"
#include "Editor/EditorEngine.h"
#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SweetDreamsCore.generated.h"

class USweetDreamsSave;

UENUM(BlueprintType)
enum class EPrintType : uint8
{
	INFO,
	WARNING,
	ERROR
};

USTRUCT(BlueprintType)
struct FDreamUserSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FIntPoint CurrentResolution;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (ClampMin = 0, ClampMax = 2))
	int32 GQuality = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (ClampMin = 0, ClampMax = 2))
	int32 GViewDistance = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (ClampMin = 0, ClampMax = 2))
	int32 GAntiAliasing = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (ClampMin = 0, ClampMax = 2))
	int32 GPostProcessing = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (ClampMin = 0, ClampMax = 2))
	int32 GShadows = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (ClampMin = 0, ClampMax = 2))
	int32 GTextures = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (ClampMin = 0, ClampMax = 2))
	int32 GEffects = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bShowFps = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bEnableVsync = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bEnableGodMode = true;

	FDreamUserSettings() {}
	FDreamUserSettings(int32 GeneralQuality)
		: GQuality(GeneralQuality),
		GViewDistance(GeneralQuality),
		GAntiAliasing(GeneralQuality),
		GPostProcessing(GeneralQuality),
		GShadows(GeneralQuality),
		GTextures(GeneralQuality),
		GEffects(GeneralQuality)
	{}

	void ApplySettings()
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetScreenResolution(CurrentResolution);

			UserSettings->SetOverallScalabilityLevel(GQuality);
			UserSettings->SetViewDistanceQuality(GViewDistance);
			UserSettings->SetAntiAliasingQuality(GAntiAliasing);
			UserSettings->SetPostProcessingQuality(GPostProcessing);
			UserSettings->SetShadowQuality(GShadows);
			UserSettings->SetTextureQuality(GTextures);
			UserSettings->SetVisualEffectQuality(GEffects);
			UserSettings->SetVSyncEnabled(bEnableVsync);
			if (bShowFps)
			{
				// Implement logic to show FPS, if applicable
			}
			if (bEnableGodMode)
			{
				// Implement logic to enable god mode
			}
			UserSettings->ApplySettings(false);
		}
	}
};

UCLASS(Category = "SweetDreams|Core")
class SWEETDREAMS_API USweetDreamsCore : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	USweetDreamsCore();
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void LoadSettings();

	// DEBUG
	void PrintDream(const UObject* DreamOrigin, FString Dream, EPrintType Severity = EPrintType::INFO, float duration = 4.0f);

	// SETTINGS
	UPROPERTY(BlueprintReadOnly)
	const USweetDreamsSettings* CoreSettings;
	void SetUserSettings(FDreamUserSettings Settings);
	FDreamUserSettings GetUserSettings() const;

	// SAVE
	bool CreateSave(TSubclassOf<USweetDreamsSaveFile> SaveClass, bool bIsPersistent = true);
	bool Save(USweetDreamsSaveFile* SaveObject, bool bIsPersistent = true);
	USweetDreamsSaveFile* LoadSave(bool bIsPersistent = true);
	void ManageSaveData(bool isSaving = true, bool bIsPersistent = true);
	UPROPERTY()
	USweetDreamsSavePersistent* SavePersistentRef = nullptr;
	UPROPERTY()
	USweetDreamsSaveLocal* SaveLocalRef = nullptr;

protected:
	// SETTINGS
	UPROPERTY()
	FDreamUserSettings UserSettings;

	// SAVE
	UPROPERTY()
	TSubclassOf<USweetDreamsSavePersistent> SaveClassPersistent = nullptr;
	UPROPERTY()
	TSubclassOf<USweetDreamsSaveLocal> SaveClassLocal = nullptr;
	FString SaveSlotPersistent = "SweetDream_PERSISTENT";
	FString SaveSlotLocal = "SweetDream_LOCAL";
};
