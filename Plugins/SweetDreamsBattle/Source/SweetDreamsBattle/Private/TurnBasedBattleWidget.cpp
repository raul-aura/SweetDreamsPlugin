// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedBattleWidget.h"

void UTurnBasedBattleWidget::SetBattleManager(ATurnBasedBattle* Battle)
{
	if (!Battle)
	{
		return;
	}
	BattleManager = Battle;
}
