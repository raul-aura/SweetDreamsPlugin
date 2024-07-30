// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleAction.h"
#include "BattleInputAction.generated.h"

class UTurnBasedBattleWidget;

UCLASS(NotBlueprintable)
class SWEETDREAMSBATTLE_API UBattleInputAction : public UBattleAction
{
	GENERATED_BODY()
	
public:
	UBattleInputAction();

	virtual void StartAction(bool bUseCooldown) override;
	UFUNCTION()
	virtual void SetWidget(UTurnBasedBattleWidget* Widget);

protected:
	UPROPERTY()
	UTurnBasedBattleWidget* TurnBattleWidget;
};
