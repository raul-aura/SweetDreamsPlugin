// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SweetDreamsInteractInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USweetDreamsInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class SWEETDREAMS_API ISweetDreamsInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	void OnInteract();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	bool IsInteractable() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Sweet Dreams|Interact")
	FText GetInteractableName() const;
};

