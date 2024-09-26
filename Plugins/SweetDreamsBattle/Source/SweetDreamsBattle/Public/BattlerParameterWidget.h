// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattlerParameterWidget.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattlerParameterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateParameters(class UBattlerDataComponent* BattlerParameters);
};

