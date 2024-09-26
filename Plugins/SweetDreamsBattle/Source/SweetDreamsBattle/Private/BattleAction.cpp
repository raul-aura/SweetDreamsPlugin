// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleAction.h"
#include "BattleCharacter.h"
#include "TurnBasedBattle.h"
#include "BattleState.h"
#include "SweetDreamsBPLibrary.h"

void UBattleAction::StartAction(bool bUseCooldown)
{
	if (!GetOwner() || bSkipThis || GetOwner()->GetBattlerParameters()->IsDead()) return;
	OnActionStart();
}

void UBattleAction::StartActionForced(bool bUseCooldown, int32 Turn)
{
	if (!CurrentBattle)
	{
		int32 BattleId = -1;
		CurrentBattle = ASweetDreamsBattleManager::FindActiveBattle(Owner, BattleId);
	}
	if (bTurnBasedAction)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (TurnBattle)
		{
			if (Turn < 0)
			{
				TurnBattle->AddTurnAction(this, !bAddedLast);
			}
			if (bUseCooldown)
			{
				RefreshCooldown();
			}
			TurnToStart = Turn;
			UBattleAction* ThisAction = this;
			TurnBattle->GetTargetsAllPossible(ThisAction);
			SetTargetRandom(ElementTargets, TargetAmount);
			return;
		}
	}
	StartAction(bUseCooldown);
}

void UBattleAction::RefreshCooldown()
{
	if (!Owner) return;
	bIsOnCooldown = true;
	TurnsPassed = -1;
	if (!bTurnBasedAction)
	{
		Owner->GetWorldTimerManager().SetTimer(ActionCooldown, this, &UBattleAction::UpdateCooldown, Cooldown, false);
	}
	else
	{
		if (CooldownTurns <= 0)
		{
			bIsOnCooldown = false;
			return;
		}
		UpdateCooldown();
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

float UBattleAction::GetPriorityWeight() const
{
	return PriorityWeigth;
}

void UBattleAction::RemoveSelfBattle()
{
	if (bTurnBasedAction && CurrentBattle)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (TurnBattle)
		{
			bSkipThis = true;
			TurnBattle->RemoveTurnAction(this);
		}
	}
}

void UBattleAction::EndAction(float Delay)
{
	if (Delay == 0.0f)
	{
		Delay = 0.01f;
	}
	FTimerHandle LocalTimer;
	Owner->GetWorldTimerManager().SetTimer(LocalTimer, [this](){
		OnActionEnd();
	}, Delay, false);
	if (Owner)
	{
		TArray<UBattleState*> States = Owner->GetAllStates();
		if (States.Num() > 0)
		{
			for (UBattleState* State : States)
			{
				State->ConsumeLifetime(EStateLifetime::Action);
				State->OnActionEnd(this);
			}
		}
		Owner->GetWorldTimerManager().ClearTimer(ActionTimer);
	}
}

void UBattleAction::StartNextTurnAction(float Delay)
{
	FTimerHandle LocalTimer;
	if (bTurnBasedAction && CurrentBattle)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (TurnBattle && Owner)
		{
			Owner->GetWorldTimerManager().SetTimer(LocalTimer, [TurnBattle, this]() {
				RemoveSelfBattle();
				TurnBattle->StartTurnAction();
				}, Delay, false);
		}
	}
}

void UBattleAction::ResetAction()
{
	ResetSkipAction();
	TurnsPassed = -1;
	bIsOnCooldown = false;
	RemoveSelfBattle();
	CurrentBattle = nullptr;
	ElementTargets.Empty();
}

void UBattleAction::ResetSkipAction()
{
	bSkipThis = false;
}

bool UBattleAction::IsActionAvailable() const
{
	float Temp = 0.f;
	float Mana = GetOwner()->GetBattlerParameters()->GetMana(Temp);
	if (Cost > Mana || bIsOnCooldown)
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

int32 UBattleAction::GetActionSpeed() const
{
	return (bOverrideOwnerSpeed) ? ActionSpeed : GetOwner()->GetBattlerParameters()->GetSpeed();
}

void UBattleAction::SetActionSpeed(int32 NewSpeed)
{
	ActionSpeed = NewSpeed;
}

float UBattleAction::GetActionCost() const
{
	return Cost;
}

float UBattleAction::StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets)
{
	float AnimationTime = Super::StartAnimation(Animation, Targets);
	FTimerHandle AnimationTimer;
	Owner->GetWorldTimerManager().SetTimer(AnimationTimer, [Animation, this] {
		OnAnimationComplete(Animation);
		}, AnimationTime, false);
	return AnimationTime;
}

void UBattleAction::MoveToTarget(ABattleCharacter* Target, int32 MovementID)
{
	if (!Owner || !Target) return;
	ASweetDreamsController* BattlerController = GetOwner()->GetDreamController();
	if (BattlerController)
	{
		BattlerController->SetCurrentMovementID(MovementID);
		BattlerController->OnDreamMovementCompleted.AddUniqueDynamic(this, &UBattleAction::OnMovementComplete);
		BattlerController->MoveToActor(Target);
	}
}

void UBattleAction::ReturnToPosition(float Delay)
{
	if (!Owner) return;
	if (Delay <= 0.0f) Delay = GetOwner()->GetWorld()->GetDeltaSeconds();
	FTimerHandle ReturnTimer;
	Owner->GetWorldTimerManager().SetTimer(ReturnTimer, [this] {
		GetOwner()->GetDreamController()->ResetPosition();
		}, Delay, false);
}

void UBattleAction::PlayLevelSequence(ULevelSequence* Sequence)
{
	if (!Sequence || !CurrentBattle) return;
	ALevelSequenceActor* OutActor;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(), OutActor);
	if (SequencePlayer)
	{
		SequencePlayer->Play();
		float Time = SequencePlayer->GetDuration().AsSeconds();
		FTimerHandle SequenceTimer;
		Owner->GetWorldTimerManager().SetTimer(SequenceTimer, [Sequence, this]() {
			OnSequenceComplete(Sequence);
			}, Time, false);
	}
}
