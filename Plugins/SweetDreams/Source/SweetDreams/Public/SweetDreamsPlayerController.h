// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SweetDreamsPlayerController.generated.h"


UCLASS()
class SWEETDREAMS_API ASweetDreamsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams")
	class ASweetDreamsCharacter* GetDreamCharacter() const;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams")
	void DisableInputTimer(float Duration = 1.f);
};
