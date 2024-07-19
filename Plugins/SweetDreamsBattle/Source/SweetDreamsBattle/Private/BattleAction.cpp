// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "TurnBasedBattle.h"

void UBattleAction::UpdateTimer(float Delay)
{
	if (Owner)
	{
		float TotalTimer = Delay;
		if (Owner->GetWorldTimerManager().TimerExists(ActionTimer))
		{
			TotalTimer += Owner->GetWorldTimerManager().GetTimerElapsed(ActionTimer);
		}
		Owner->GetWorldTimerManager().SetTimer(ActionTimer, this, &UBattleAction::OnActionEnd, TotalTimer, false);
	}
}

void UBattleAction::EndAction(float Delay)
{
	if (Delay == 0.0f)
	{
		Delay = 0.01f;
	}
	FTimerHandle LocalTimer;
	if (bTurnBasedAction && CurrentBattle)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (TurnBattle && Owner)
		{
			Owner->GetWorldTimerManager().SetTimer(LocalTimer, TurnBattle, &ATurnBasedBattle::StartActionInOrder, Delay, false);
		}
	}
	if (Owner)
	{
		Owner->GetWorldTimerManager().ClearTimer(ActionTimer);
	}
}

void UBattleAction::ResetAction()
{
	CurrentBattle = nullptr;
	ElementTargets.Empty();
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
