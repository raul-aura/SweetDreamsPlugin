// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleAction.h"
#include "BattleCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TurnBasedBattle.h"

void UBattleAction::StartAction(bool bUseCooldown)
{
	OnActionStart();
	if (bUseCooldown)
	{
		RefreshCooldown();
	}
	if (!Owner) return;
	OwnerTransform = GetOwner()->GetTransform();
}

void UBattleAction::StartActionForced(bool bUseCooldown)
{
	if (bTurnBasedAction && CurrentBattle)
	{
		ATurnBasedBattle* TurnBattle = Cast<ATurnBasedBattle>(CurrentBattle);
		if (TurnBattle)
		{
			TurnBattle->AddTurnAction(this, !bAddedLast);
			if (bUseCooldown)
			{
				RefreshCooldown();
			}
		}
		UBattleAction* ThisAction = this;
		TurnBattle->GetTargetsAllPossible(ThisAction);
		SetTargetRandom(ElementTargets, TargetAmount);
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
	if (!Owner) return;
	bIsOnCooldown = true;
	TurnsPassed = 0;
	if (!bTurnBasedAction)
	{

		Owner->GetWorldTimerManager().SetTimer(ActionCooldown, this, &UBattleAction::UpdateCooldown, Cooldown, false);
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

void UBattleAction::UpdateTimer(float Delay)
{
	if (!Owner) return;
	float TotalTimer = Delay;
	if (Owner->GetWorldTimerManager().TimerExists(ActionTimer))
	{
		TotalTimer += Owner->GetWorldTimerManager().GetTimerElapsed(ActionTimer);
	}
	Owner->GetWorldTimerManager().SetTimer(ActionTimer, this, &UBattleAction::OnActionEnd, TotalTimer, false);
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
			Owner->GetWorldTimerManager().SetTimer(LocalTimer, TurnBattle, &ATurnBasedBattle::StartTurnAction, Delay, false);
		}
	}
	if (Owner)
	{
		TArray<UBattleState*> States = Owner->GetAllStates();
		if (States.Num() > 0)
		{
			for (UBattleState* State : States)
			{
				State->ConsumeLifetime(EStateLifetime::Action);
				State->OnActionEnd();
			}
		}
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
	float Mana = GetOwner()->GetBattlerParameters()->GetMana();
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
	UpdateTimer(AnimationTime);
	return AnimationTime;
}

void UBattleAction::MoveToTarget(ABattleCharacter* Target)
{
	if (!Owner || !Target) return;
	FVector StartLocation = OwnerTransform.GetLocation();
	FVector EndLocation = Target->GetActorLocation();
	float Distance = FVector::Dist(StartLocation, EndLocation);
	float Speed = GetOwner()->GetCharacterMovement()->MaxWalkSpeed;
	float Time = 0.0f;
	if (Speed > 0.0f) Time = Distance / Speed;
	UpdateTimer(Time);
	GetOwner()->MoveToLocation(EndLocation);
}

void UBattleAction::ReturnToPosition()
{
	if (!Owner) return;
	GetOwner()->MoveToLocation(OwnerTransform.GetLocation());
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
		UpdateTimer(Time);
	}
}
