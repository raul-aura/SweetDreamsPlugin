// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleElement.h"
#include "BattleCharacter.h"
#include "BattlerDataComponent.h"

void UBattleElement::SetOwner(ABattleCharacter* InputOwner)
{
	Owner = InputOwner;
}

ABattleCharacter* UBattleElement::GetOwner() const
{
	return Owner;
}

FText UBattleElement::GetName() const
{
	return ElementName;
}

void UBattleElement::SetBattle(ASweetDreamsBattleManager* Battle)
{
	CurrentBattle = Battle;
}

void UBattleElement::AddTarget(ABattleCharacter* Target)
{
	ElementTargets.Add(Target);
}

void UBattleElement::SetTarget(TArray<ABattleCharacter*> NewTargets)
{
	ElementTargets = NewTargets;
}

bool UBattleElement::DamageTargets(TArray<ABattleCharacter*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage, bool bCanBeMitigated)
{
	PostMitigatedDamage = 0.0f;
	KilledTargets = 0;
	if (Targets.Num() == 0)
	{
		return false;
	}
	bool bAllDead = true;
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			if (UBattlerDataComponent* Data = Target->GetBattlerParameters())
			{
				if (Data->IsDead() || Target->IsPendingKill())
				{
					continue;
				}
				PostMitigatedDamage = Data->ReceiveDamage(Damage, bCanBeMitigated);
				if (!Data->IsDead())
				{
					bAllDead = false;
					continue;
				}
				KilledTargets++;
			}
		}
	}
	return bAllDead;
}

void UBattleElement::HealTargets(TArray<ABattleCharacter*> Targets, float& HealedAmount, float& OverhealAmount, float Heal)
{
	HealedAmount = 0.0f;
	OverhealAmount = 0.0f;
	if (Targets.Num() == 0)
	{
		return;
	}
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			if (UBattlerDataComponent* Data = Target->GetBattlerParameters())
			{
				if (Data->IsDead() || Target->IsPendingKill())
				{
					continue;
				}
				HealedAmount = Data->ReceiveHeal(Heal);
				OverhealAmount = Heal - HealedAmount;
			}
		}
	}
}
