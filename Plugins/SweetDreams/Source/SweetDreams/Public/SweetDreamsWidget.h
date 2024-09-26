// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SweetDreamsWidget.generated.h"

UCLASS()
class SWEETDREAMS_API USweetDreamsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName WidgetName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIgnoreThisForVisibility = false;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnShow();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnHide();
	UFUNCTION(BlueprintCallable)
	void HideSelf();
};

