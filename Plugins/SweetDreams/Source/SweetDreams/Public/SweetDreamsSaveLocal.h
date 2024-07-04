// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsSaveFile.h"
#include "SweetDreamsSaveLocal.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SWEETDREAMS_API USweetDreamsSaveLocal : public USweetDreamsSaveFile
{
	GENERATED_BODY()

public:
	virtual void OnSaveLoaded(TArray<AActor*>& Actors) override;
	virtual void OnSaveSaved(TArray<AActor*>& Actors) override;
};
