// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsCore.h"
#include "SweetDreamsSaveInterface.h"
#include "GameFramework/SaveGame.h"
#include "SweetDreamsSaveFile.generated.h"

UCLASS(Abstract, NotBlueprintable)
class SWEETDREAMS_API USweetDreamsSaveFile : public USaveGame
{
	GENERATED_BODY()

public:
	USweetDreamsSaveFile();

	virtual void OnSaveLoaded(TArray<AActor*>& Actors);
	virtual void OnSaveSaved(TArray<AActor*>& Actors);
};
