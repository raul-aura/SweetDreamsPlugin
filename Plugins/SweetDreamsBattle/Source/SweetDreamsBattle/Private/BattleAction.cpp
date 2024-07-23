// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "TurnBasedBattle.h"

void UBattleAction::StartAction(bool bUseCooldown)
{
	OnActionStart();
	if (bUseCooldown)
	{
		RefreshCooldown();
	}
}

void UBattleAction::StartActionForced(bool bUseCooldown)
{
	if (bTurnBasedAction && CurrentBattle)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (TurnBattle)
		{
			TurnBattle->AddActionAuto(this, bAddedLast);
			if (bUseCooldown)
			{
				RefreshCooldown();
			}
		}
		TurnBattle->UpdatePossibleTargets(this);
		SetTargetRandom(TurnBattle->GetPossibleTargets(), TargetAmount);
		return;
	}
	OnActionStart();
	if (bUseCooldown)
	{
		RefreshCooldown();
	}
}

void UBattleAction::RefreshCooldown()
{
	if (Owner)
	{
		bIsOnCooldown = true;
		TurnsPassed = 0;
		if (!bTurnBasedAction)
		{

			Owner->GetWorldTimerManager().SetTimer(ActionCooldown, this, &UBattleAction::UpdateCooldown, Cooldown, false);
		}
	}
}

void UBattleAction::UpdateCooldown()
{
	if (bIsOnCooldown)
	{
		if (!bTurnBasedAction)
		{
			bIsOnCooldown = false;
		}
		else
		{
			TurnsPassed = FMath::Clamp(TurnsPassed + 1, 0, CooldownTurns);
			if (TurnsPassed >= CooldownTurns)
			{
				bIsOnCooldown = false;
			}
		}
	}
}

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
	TurnsPassed = 0;
	bIsOnCooldown = false;
	CurrentBattle = nullptr;
	ElementTargets.Empty();
}

bool UBattleAction::IsActionAvailable() const
{
	//Owner->GetResource()
	if (Cost > 0 || bIsOnCooldown)
	{
		return false;
	}
	return true;
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

float UBattleAction::StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets)
{
	float AnimationTime = Super::StartAnimation(Animation, Targets);
	UpdateTimer(AnimationTime);
	return AnimationTime;
}
