// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsSavePersistent.h"
#include "SweetDreamsSaveLocal.h"
#include "SweetDreamsCore.h"
#include "SweetDreamsState.h"
#include "Engine/DeveloperSettings.h"
#include "SweetDreamsSettings.generated.h"

class ASweetDreamsState;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDebugFlags : uint8
{
	NONE = 0 UMETA(Hidden),
	PrintEnabled = 1 << 0,
	PrintSaveOperations = 1 << 1,
	PrintStateOperations = 1 << 2,
	PrintStateCreated = 1 << 3,
};
ENUM_CLASS_FLAGS(EDebugFlags)

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Sweet Dreams Core"))
class SWEETDREAMS_API USweetDreamsSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public: 
	USweetDreamsSettings();

	// DEBUG
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Debug", meta = (Bitmask, BitmaskEnum = EDebugFlags))
	int32 DebugFlags;

	// GAME
	
	// Default Dream States. Check documentation for more info. 
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|State", meta = (Tooltip = "Add or create your custom Dream States classes here. For more information on the default Dream States, check documentation."))
	TArray<TSubclassOf<ASweetDreamsState>> DreamStates;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|State", meta = (Tooltip = "If true, you can specify what Dream State will always be starting with Sweet Dreams Game Mode.\n\nBy default, the state that is always started is Awake."))
	bool bOverrideInitialState = false;
	//The State Name of the new state to be started.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|State", meta = (EditCondition = "bOverrideInitialState"))
	FName NewInitialState = "Awake";

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Settings", meta = (Tooltip = "On Core initialize, the Persistent Save and Local Save will always be created and saved.\n\nSince these functions run on Game Instance Initialize, it cannot call Actor interfaces such as load/save data."))
	bool bEnableAutoCreateSave = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Settings", meta = (Tooltip = "On Core initialize, the Persistent Save file will always be loaded, if any.\n\nSince these functions run on Game Instance Initialize, it cannot call Actor interfaces such as load/save data."))
	bool bEnableAutoLoadSave = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Game|Settings", meta = (Tooltip = "On Game Mode Begin Play, the Persistent Save file will always load data to actors with Sweet Dreams Save Interface, if any.\n\nYour Game Mode needs to be an instance of SweetDreamsGameMode for this to work."))
	bool bEnableAutoLoadData = true;

	// SAVE
	
	//Name of the Persistent Save File
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Persistent", meta = (DisplayName = "Persistent File Name"))
	FString PersistentSlot = "SweetDream_PERSISTENT";
	//Class of the Persistent Save
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Persistent", meta = (DisplayName = "Persistent Save Class"))
	TSubclassOf<USweetDreamsSavePersistent> PersistentClass = USweetDreamsSavePersistent::StaticClass();
	//Name of the Local Save File
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Local", meta = (DisplayName = "Local File Name"))
	FString LocalSlot = "SweetDream_LOCAL";
	//Class of the Local Save
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Save|Local", meta = (DisplayName = "Local Save Class"))
	TSubclassOf<USweetDreamsSaveLocal> LocalClass = USweetDreamsSaveLocal::StaticClass();
};
