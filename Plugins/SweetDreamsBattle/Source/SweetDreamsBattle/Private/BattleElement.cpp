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

FText UBattleElement::GetElementName() const
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
	ElementTargets.Empty();
	ElementTargets = NewTargets;
}

void UBattleElement::SetTargetRandom(TArray<ABattleCharacter*> PossibleTargets, int32 TargetAmount)
{
	if (PossibleTargets.Num() == 0 || TargetAmount <= 0)
	{
		return;
	}
	ElementTargets.Empty();
	int32 RandomIndex = FMath::RandRange(0, PossibleTargets.Num() - 1);
	for (int32 i = 0; i < TargetAmount; i++)
	{
		AddTarget(PossibleTargets[RandomIndex]);
	}
}

bool UBattleElement::DamageTargets(TArray<ABattleCharacter*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage, bool bCanBeMitigated)
{
	PostMitigatedDamage = 0.0f;
	KilledTargets = 0;
	if (!AreTargetsValid(Targets))
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
	if (!AreTargetsValid(Targets))
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

void UBattleElement::KillTargets(TArray<ABattleCharacter*> Targets)
{
	if (!AreTargetsValid(Targets))
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
				Data->Kill();
			}
		}
	}
}

void UBattleElement::ReviveTargets(TArray<ABattleCharacter*> Targets)
{
	if (!AreTargetsValid(Targets))
	{
		return;
	}
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			if (UBattlerDataComponent* Data = Target->GetBattlerParameters())
			{
				Data->Revive();
			}
		}
	}
}

float UBattleElement::StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets)
{
	if (!AreTargetsValid(Targets) || !Animation)
	{
		return 0.0f;
	}
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			if (USkeletalMeshComponent* Mesh = Target->GetMesh())
			{
				Mesh->PlayAnimation(Animation, false);
			}
		}
	}
	return Animation->GetPlayLength();
}

void UBattleElement::ForceAction(TSubclassOf<UBattleAction> Action, TArray<ABattleCharacter*> Targets, bool bUseCooldown)
{
	if (!AreTargetsValid(Targets) || !Action)
	{
		return;
	}
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			UBattleAction* NewAction = NewObject<UBattleAction>(Target, Action);
			if (NewAction)
			{
				NewAction->SetOwner(Target);
				NewAction->StartActionForced(bUseCooldown);
			}
		}
	}
}

bool UBattleElement::AreTargetsValid(TArray<ABattleCharacter*>& Targets)
{
	if (Targets.Num() == 0)
	{
		if (ElementTargets.Num() == 0)
		{
			return false;
		}
		Targets = ElementTargets;
	}
	return true;
}
