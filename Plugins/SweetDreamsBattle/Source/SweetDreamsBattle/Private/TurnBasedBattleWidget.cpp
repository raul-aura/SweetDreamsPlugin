// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedBattleWidget.h"
#include "TimerManager.h"

void UTurnBasedBattleWidget::ShowPlayerInputDelayed(float Delay)
{
	if (Delay <= 0.0f)
	{
		Delay = GetWorld()->GetDeltaSeconds();
	}
	GetWorld()->GetTimerManager().SetTimer(InputTimer, this, &UTurnBasedBattleWidget::ShowPlayerInput, Delay, false);
}

void UTurnBasedBattleWidget::SetBattleManager(ATurnBasedBattle* Battle)
{
	if (!Battle)
	{
		return;
	}
	BattleManager = Battle;
}
