// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleState.h"
#include "BattleCharacter.h"

ABattleCharacter* UBattleState::GetStateInstigator() const
{
	return StateInstigator;
}

TArray<ABattleCharacter*> UBattleState::GetInstigatorAsArray() const
{
	TArray<ABattleCharacter*> ArrayInstigator;
	ArrayInstigator.Add(GetStateInstigator());
	return ArrayInstigator;
}

void UBattleState::ApplyState(ABattleCharacter* Instigator)
{
	if (!Instigator) return;
	StateInstigator = Instigator;
	AddStacks();
	ResetLifetime();
	OnApplied();
	bAlreadyAppliedOnce = true;
}

void UBattleState::ResetLifetime()
{
	IntLifetime = 0;
	FloatLifetime = 0.0f;
	switch (Lifetime)
	{
	case EStateLifetime::Turn:
		IntLifetime = TurnsToEnd;
		break;
	case EStateLifetime::Action:
		IntLifetime = ActionsToEnd;
		break;
	case EStateLifetime::Second:
		FloatLifetime = SecondsToEnd;
		break;
	default:
		break;
	}
}

void UBattleState::AddStacks()
{
	Stacks = (Stacks == 0) ? InitialStacks : Stacks + 1;
	if (!bIsStackable)
	{
		MaxStacks = InitialStacks;
	}
	Stacks = FMath::Clamp(Stacks, InitialStacks, MaxStacks);
}

void UBattleState::ConsumeStacks()
{
	if (!bConsumeStacks) return;
	int32 LocalMinStacks = (Lifetime == EStateLifetime::Permanent) ? InitialStacks : 0;
	Stacks = FMath::Clamp(--Stacks, LocalMinStacks, MaxStacks);
	if (Stacks == 0)
	{
		RemoveState();
	}
}

void UBattleState::ConsumeLifetime(EStateLifetime LifetimeToConsume)
{
	if (Lifetime == EStateLifetime::Permanent || LifetimeToConsume != Lifetime) return;
	if (IntLifetime == 0 && FloatLifetime == 0.0f)
	{
		RemoveState();
	}
	switch (Lifetime)
	{
	case EStateLifetime::Turn:
		IntLifetime = FMath::Clamp(--IntLifetime, 0, TurnsToEnd);
		break;
	case EStateLifetime::Action:
		IntLifetime = FMath::Clamp(--IntLifetime, 0, ActionsToEnd);
		break;
	case EStateLifetime::Second:
		FloatLifetime = FMath::Clamp(FloatLifetime - GetWorld()->GetDeltaSeconds(), 0.0f, SecondsToEnd);
		break;
	default:
		break;
	}
	if (bAutoConsumeStacks)
	{
		ConsumeStacks();
	}
}

void UBattleState::RemoveState()
{
	if (Owner)
	{
		Owner->RemoveState(GetClass());
	}
}

int32 UBattleState::GetRemainingLifetime() const
{
	return IntLifetime + 1;
}

float UBattleState::GetRemainingLifetimeSeconds() const
{
	return FloatLifetime;
}
