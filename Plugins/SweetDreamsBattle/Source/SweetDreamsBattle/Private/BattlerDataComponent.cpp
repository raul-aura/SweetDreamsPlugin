// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlerDataComponent.h"
#include "BattleCharacter.h"

UBattlerDataComponent::UBattlerDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentHealth = Health;
}

void UBattlerDataComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBattlerDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UBattlerDataComponent::GetModifiedParameter(TArray<float> Modifiers, float Parameter, float BaseMultiplier) const
{
	if (BaseMultiplier < 0.0f || Parameter < 0.0f) return 0.0f;
	float MultipliedParam = Parameter * (BaseMultiplier / 100);
	Modifiers.Add(MultipliedParam);
	float AllModifiers = 0.0f;
	for (float Modifier : Modifiers)
	{
		AllModifiers += Modifier;
	}
	AllModifiers = FMath::Max(AllModifiers, 0.0f);
	return AllModifiers;
}

ABattleCharacter* UBattlerDataComponent::GetBattlerOwner() const
{
	return Cast<ABattleCharacter>(GetOwner());
}

float UBattlerDataComponent::GetHealth() const
{
	return GetModifiedParameter(HealthModifiers, CurrentHealth, HealthMultiplier);
}

float UBattlerDataComponent::GetMana() const
{
	return CurrentMana;
}

float UBattlerDataComponent::GetForce() const
{
	return GetModifiedParameter(ForceModifiers, Force, ForceMultiplier);
}

float UBattlerDataComponent::GetResistence() const
{
	return GetModifiedParameter(ResistenceModifiers, Resistence, ResistenceMultiplier);
}

int32 UBattlerDataComponent::GetSpeed() const
{
	return Speed;
}

int32 UBattlerDataComponent::GetAdditionalActions() const
{
	return AdditionalActions;
}

int32 UBattlerDataComponent::AddModifier(TArray<float>& Modifiers, float ModifierToAdd)
{
	if (ModifierToAdd < 0.0f) return -1;
	return Modifiers.Add(ModifierToAdd);
}

bool UBattlerDataComponent::UpdateModifier(TArray<float>& Modifiers, float ModifierToSearch, float NewModifier)
{
	if (ModifierToSearch < 0.0f || NewModifier < 0.0f) return false;
	int32 FoundIndex;
	if (Modifiers.Find(ModifierToSearch, FoundIndex))
	{
		Modifiers[FoundIndex] = NewModifier;
		return true;
	}
	return false;
}

bool UBattlerDataComponent::UpdateModifierAt(TArray<float>& Modifiers, int32 Index, float NewModifier)
{
	if (Index < 0 || NewModifier < 0.0f) return false;
	if (Modifiers.IsValidIndex(Index))
	{
		Modifiers[Index] = NewModifier;
		return true;
	}
	return false;
}

bool UBattlerDataComponent::RemoveModifier(TArray<float>& Modifiers, float ModifierToRemove)
{
	if (ModifierToRemove < 0.0f) return false;
	if (Modifiers.Find(ModifierToRemove))
	{
		Modifiers.Remove(ModifierToRemove);
		return true;
	}
	return false;
}

bool UBattlerDataComponent::RemoveModifierAt(TArray<float>& Modifiers, int32 Index)
{
	if (Index < 0) return false;
	if (Modifiers.IsValidIndex(Index))
	{
		Modifiers.RemoveAt(Index);
		return true;
	}
	return false;
}

float UBattlerDataComponent::ReceiveDamage(float Damage, bool bCanBeMitigated)
{
	if (Damage < 0) Damage *= -1;
	float FinalDamage = Damage;
	if (bCanBeMitigated)
	{
		FinalDamage = GetBattlerOwner()->OnDamageReceived(FinalDamage, bCanBeMitigated);
	}
	CurrentHealth = FMath::Clamp(CurrentHealth - FinalDamage, 0.0f, Health);
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		Kill();
	}
	return FinalDamage;
}

float UBattlerDataComponent::ReceiveHeal(float Heal)
{
	if (Heal < 0) Heal *= -1;
	float HealedAmount = FMath::Clamp(Heal, 0.0f, (Health - CurrentHealth));
	CurrentHealth += HealedAmount;
	return HealedAmount;
}

float UBattlerDataComponent::ReceiveManaConsume(float Consume)
{
	if (Consume < CurrentMana || Consume == 0) return CurrentMana;
	if (Consume < 0) Consume *= -1;
	CurrentMana = FMath::Clamp(CurrentMana - Consume, 0.0f, Mana);
	return CurrentMana;
}

float UBattlerDataComponent::ReceiveManaRestore(float Restore)
{
	return 0.0f;
}

void UBattlerDataComponent::Kill()
{
	bIsDead = true;
	GetBattlerOwner()->OnKilled();
}

void UBattlerDataComponent::Revive()
{
	if (IsDead())
	{
		bIsDead = false;
	}
	GetBattlerOwner()->OnRevived();
}

bool UBattlerDataComponent::IsDead() const
{
	return bIsDead;
}

