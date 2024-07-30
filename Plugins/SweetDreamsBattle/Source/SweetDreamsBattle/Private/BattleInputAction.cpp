// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleInputAction.h"
#include "TurnBasedBattleWidget.h"

UBattleInputAction::UBattleInputAction()
{
	ElementName = FText::FromString(TEXT("Input"));
	bOverrideOwnerSpeed = true;
	bTurnBasedAction = true;
	ActionSpeed = 999;
	Cost = 0;
}

void UBattleInputAction::StartAction(bool bUseCooldown)
{
	if (TurnBattleWidget)
	{
		TurnBattleWidget->ShowPlayerInput();
	}
}

void UBattleInputAction::SetWidget(UTurnBasedBattleWidget* Widget)
{
	TurnBattleWidget = Widget;
}
