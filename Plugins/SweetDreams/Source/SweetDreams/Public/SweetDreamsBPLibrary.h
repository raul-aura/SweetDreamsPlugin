// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SweetDreamsCore.h"
#include "SweetDreamsBPLibrary.generated.h"

class USweetDreamsCore;
class ASweetDreamsGameMode;

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class USweetDreamsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	// DEBUG
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "DreamOrigin", Keywords = "Print SweetDreams core dream debug string text", CallableWithoutWorldContext), Category = "SweetDreams|Core|DEBUG")
	static void PrintDream(const UObject* DreamOrigin, FString Dream = "Hello dream.", EPrintType Severity = EPrintType::INFO, float duration = 4.0f);
	// GETTERS
	UFUNCTION(meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsCore* GetSweetDreamsCore(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsGameMode* GetSweetDreamsGameMode(const UObject* WorldContext);
	// SAVE
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static bool CreateSaveGame(const UObject* WorldContext, TSubclassOf<USweetDreamsSaveFile> SaveClass, bool bIsPersistent = true);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static bool SaveGame(const UObject* WorldContext, USweetDreamsSaveFile* SaveObject, bool bIsPersistent = true);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsSaveFile* LoadSaveGame(const UObject* WorldContext, bool bIsPersistent = true);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsSavePersistent* GetPersistentSave(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static USweetDreamsSaveLocal* GetLocalSave(const UObject* WorldContext);
	// SETTINGS
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static FDreamUserSettings GetUserSettings(const UObject* WorldContext);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void SetUserSettings(const UObject* WorldContext, FDreamUserSettings Settings);
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static void SetSettingsQuality(const UObject* WorldContext, int32 Quality);

	static USweetDreamsCore* SweetDreamsCore;
};
