// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleNumberWidget.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API UBattleNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void IndicateHeal(float Heal);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void IndicateDamage(float Damage);
	UFUNCTION(BlueprintCallable)
	void RemoveThisIndicator();

	UPROPERTY(BlueprintReadOnly)
	class UWidgetComponent* OwningWidgetComponent;
	UPROPERTY(BlueprintReadOnly)
	class ABattleCharacter* OwningBattler;

};

