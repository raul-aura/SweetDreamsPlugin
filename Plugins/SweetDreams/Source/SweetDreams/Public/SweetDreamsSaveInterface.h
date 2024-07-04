// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SweetDreamsSaveFile.h"
#include "SweetDreamsSaveInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USweetDreamsSaveInterface : public UInterface
{
	GENERATED_BODY()
};

class SWEETDREAMS_API ISweetDreamsSaveInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save") //Calling this on BeginPlay or InitializeCore will always return an empty save file.\n\nRemember that Local Save is meant for on runtime data only and will always be destroyed on game end (deinitialize).
	void OnLocalLoaded(USweetDreamsSaveLocal* SaveFile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save") //Calling this on BeginPlay or InitializeCore will always return an empty save file.\n\nTo always load persistent data on start, call it on Level Blueprint or Game Mode.
	void OnPersistentLoaded(USweetDreamsSavePersistent* SaveFile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	void OnLocalSaved(USweetDreamsSaveLocal* SaveFile);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Sweet Dreams|Save")
	void OnPersistentSaved(USweetDreamsSavePersistent* SaveFile);
};
