// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleElement.h"
#include "BattleCharacter.h"
#include "SweetDreamsBattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "SweetDreamsBPLibrary.h"
#include "BattlerDataComponent.h"

void UBattleElement::SetOwner(ABattleCharacter* InputOwner)
{
	Owner = InputOwner;
}

ABattleCharacter* UBattleElement::GetOwner() const
{
	return Owner;
}

TArray<ABattleCharacter*> UBattleElement::GetOwnerAsArray() const
{
	TArray<ABattleCharacter*> ArrayOwner;
	ArrayOwner.Add(GetOwner());
	return ArrayOwner;
}

void UBattleElement::UpdateElementDescription(FText NewDescription)
{
	if (NewDescription.IsEmpty()) return;
	ElementDescription = NewDescription;
}

void UBattleElement::SetBattle(ASweetDreamsBattleManager* Battle)
{
	CurrentBattle = Battle;
}

void UBattleElement::AddTarget(ABattleCharacter* Target, bool bRemoveDead)
{
	if (!ElementTargets.Contains(Target))
	{
		ElementTargets.Add(Target);
	}
	if (bRemoveDead)
	{
		UpdateValidTargets();
	}
}

void UBattleElement::RemoveTarget(ABattleCharacter* Target)
{
	if (ElementTargets.Contains(Target))
	{
		ElementTargets.Remove(Target);
	}
}

void UBattleElement::SetTarget(TArray<ABattleCharacter*> NewTargets, bool bRemoveDead)
{
	ElementTargets.Empty();
	ElementTargets = NewTargets;
	if (bRemoveDead)
	{
		UpdateValidTargets();
	}
}

void UBattleElement::SetTargetRandom(TArray<ABattleCharacter*> PossibleTargets, int32 TargetAmount, bool bRemoveDead)
{
	if (PossibleTargets.Num() == 0 || TargetAmount <= 0) return;
	ElementTargets.Empty();
	int32 RandomIndex = FMath::RandRange(0, PossibleTargets.Num() - 1);
	for (int32 i = 0; i < TargetAmount; i++)
	{
		AddTarget(PossibleTargets[RandomIndex]);
	}
	if (bRemoveDead)
	{
		UpdateValidTargets();
	}
}

TArray<ABattleCharacter*> UBattleElement::GetAdjacentTargets(ABattleCharacter* PrimaryTarget, TArray<ABattleCharacter*> TargetsToSearch)
{
	TArray<ABattleCharacter*> FoundTargets;
	if (!AreTargetsValid(TargetsToSearch)) return FoundTargets;
	int32 FoundIndex = TargetsToSearch.Find(PrimaryTarget);
	if (FoundIndex != INDEX_NONE)
	{
		if (TargetsToSearch.IsValidIndex(FoundIndex - 1))
		{
			FoundTargets.Add(TargetsToSearch[FoundIndex - 1]);
		}
		if (TargetsToSearch.IsValidIndex(FoundIndex + 1))
		{
			FoundTargets.Add(TargetsToSearch[FoundIndex + 1]);
		}
	}
	return FoundTargets;
}

bool UBattleElement::UpdateValidTargets()
{
	if (ElementTargets.Num() == 0) return false;
	for (ABattleCharacter* Target : ElementTargets)
	{
		if (Target)
		{
			if (Target->GetBattlerParameters() && Target->GetBattlerParameters()->IsDead() || Target->IsPendingKill())
			{
				ElementTargets.Remove(Target);
			}
		}
	}
	return ElementTargets.Num() > 0;
}

bool UBattleElement::DamageTargets(TArray<ABattleCharacter*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage, bool bCanBeMitigated, bool bApplyCalculations)
{
	PostMitigatedDamage = 0.0f;
	KilledTargets = 0;
	if (!AreTargetsValid(Targets)) return false;
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
				int32 Index = 1;
				if (ASweetDreamsBattleManager* Battle = ASweetDreamsBattleManager::FindActiveBattle(GetOwner(), Index))
				{
					Battle->AddDamageToBattle(GetOwner(), PostMitigatedDamage, bApplyCalculations);
				}
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
	if (!AreTargetsValid(Targets)) return;
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

bool UBattleElement::AddStatesToTargets(UObject* StateInstigator, TArray<TSubclassOf<UBattleState>> States, TArray<ABattleCharacter*> Targets, int32& StatesAdded, float Chance)
{
	if (!AreTargetsValid(Targets) || !StateInstigator || States.Num() == 0) return false;
	Chance = FMath::Clamp(Chance, 0.0f, 1.0f);
	float RandomNum = 0.0f;
	bool bAllStatesApplied = true;
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
				RandomNum = FMath::FRand();
				if (Chance < RandomNum)
				{
					bAllStatesApplied = false;
					continue;
				}
				for (TSubclassOf<UBattleState> State : States)
				{
					Target->AddState(State, StateInstigator);
					StatesAdded++;
				}
			}
		}
	}
	return bAllStatesApplied;
}

bool UBattleElement::RemoveStatesOfTargets(TArray<TSubclassOf<UBattleState>> States, TArray<ABattleCharacter*> Targets, int32& StatesRemoved, float Chance)
{
	if (!AreTargetsValid(Targets) || States.Num() == 0) return false;
	Chance = FMath::Clamp(Chance, 0.0f, 1.0f);
	float RandomNum = 0.0f;
	bool bAllStatesRemoved = true;
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
				RandomNum = FMath::FRand();
				if (Chance < RandomNum)
				{
					bAllStatesRemoved = false;
					continue;
				}
				for (TSubclassOf<UBattleState> State : States)
				{
					Target->RemoveState(State);
					StatesRemoved++;
				}
			}
		}
	}
	return bAllStatesRemoved;
}

void UBattleElement::CleanseTargets(TArray<ABattleCharacter*> Targets, int32& StatesRemoved)
{
	if (!AreTargetsValid(Targets)) return;
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
				StatesRemoved = Target->RemoveAllStates();
			}
		}
	}
}

void UBattleElement::KillTargets(TArray<ABattleCharacter*> Targets)
{
	if (!AreTargetsValid(Targets)) return;
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

void UBattleElement::ReviveTargets(TArray<ABattleCharacter*> Targets, float HealthRestore, float ManaRestore)
{
	if (!AreTargetsValid(Targets)) return;
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			if (UBattlerDataComponent* Data = Target->GetBattlerParameters())
			{
				Data->Revive(HealthRestore, ManaRestore);
			}
		}
	}
}

float UBattleElement::StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets)
{
	if (!AreTargetsValid(Targets) || !Animation) return 0.f;
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			if (USkeletalMeshComponent* Mesh = Target->GetMesh())
			{
				if (Mesh->GetAnimationMode() == EAnimationMode::AnimationBlueprint)
				{
					float PlayLength = Animation->GetPlayLength();
					FTimerHandle AnimTimer;
					GetWorld()->GetTimerManager().SetTimer(AnimTimer, [Mesh]()
						{
							Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
						}, PlayLength, false);
				}
				Mesh->PlayAnimation(Animation, false);
			}
		}
	}
	return Animation->GetPlayLength();
}

void UBattleElement::ForceAction(TSubclassOf<UBattleAction> Action, TArray<ABattleCharacter*> Targets, bool bUseCooldown, int32 Turn)
{
	if (!AreTargetsValid(Targets) || !Action) return;
	for (ABattleCharacter* Target : Targets)
	{
		if (Target)
		{
			UBattleAction* NewAction = NewObject<UBattleAction>(Target, Action);
			if (NewAction)
			{
				NewAction->SetOwner(Target);
				NewAction->StartActionForced(bUseCooldown, Turn);
			}
		}
	}
}

bool UBattleElement::AreTargetsValid(const TArray<ABattleCharacter*>& Targets)
{
	return (Targets.Num() > 0);
}
