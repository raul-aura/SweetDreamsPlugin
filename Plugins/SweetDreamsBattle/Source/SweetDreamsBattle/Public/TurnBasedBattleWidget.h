// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurnBasedBattleWidget.generated.h"

class ATurnBasedBattle;

UCLASS()
class SWEETDREAMSBATTLE_API UTurnBasedBattleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void ShowPlayerInput();
	UFUNCTION(BlueprintImplementableEvent)
	void HidePlayerInput();

	UFUNCTION(BlueprintCallable)
	void SetBattleManager(ATurnBasedBattle* Battle);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG")
	ATurnBasedBattle* BattleManager;
};
