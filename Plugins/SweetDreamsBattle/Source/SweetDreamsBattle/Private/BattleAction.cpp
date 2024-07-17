// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleAction.h"
#include "Engine/World.h"
#include "TurnBasedBattle.h"

void UBattleAction::UpdateTimer(float Delay)
{
	if (Owner)
	{
		Owner->GetWorldTimerManager().SetTimer(ActionTimer, this, &UBattleAction::EndAction, Delay, false);
	}
}

void UBattleAction::EndAction()
{
	if (bTurnBasedAction && CurrentBattle)
	{
		Cast<ATurnBasedBattle>(CurrentBattle)->StartActionInOrder();
	}
}

void UBattleAction::ResetAction()
{
	CurrentBattle = nullptr;
	Targets.Empty();
}

ETargetType UBattleAction::GetTargetType() const
{
	return TargetType;
}

int32 UBattleAction::GetTargetAmount() const
{
	return TargetAmount;
}

bool UBattleAction::GetIfIncludeSelf() const
{
	return bIncludeSelf;
}

void UBattleAction::ActionDelay(float DelayTime)
{
	UpdateTimer(DelayTime);
}
