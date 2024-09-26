// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlerDataComponent.h"
#include "BattleCharacter.h"

UBattlerDataComponent::UBattlerDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBattlerDataComponent::BeginPlay()
{
	CurrentHealth = Health;
	CurrentMana = Mana;
	CurrentLives = AdditionalLives;
	Super::BeginPlay();
}

float UBattlerDataComponent::GetModifiedParameter(TArray<float> Modifiers, float Parameter, float BaseMultiplier) const
{
	if (BaseMultiplier < 0.f || Parameter < 0.f) return 0.f;
	float MultipliedParam = Parameter * (BaseMultiplier / 100);
	Modifiers.Add(MultipliedParam);
	float AllModifiers = 0.f;
	for (float Modifier : Modifiers)
	{
		AllModifiers += Modifier;
	}
	AllModifiers = FMath::Max(AllModifiers, 0.f);
	return AllModifiers;
}

ABattleCharacter* UBattlerDataComponent::GetBattlerOwner() const
{
	return Cast<ABattleCharacter>(GetOwner());
}

float UBattlerDataComponent::GetHealth(float& MaxHealth, float Multiplier)
{
	MaxHealth = GetModifiedParameter(HealthModifiers, Health, HealthMultiplier) * (Multiplier / 100);
	return FMath::Min(GetModifiedParameter(HealthModifiers, CurrentHealth, HealthMultiplier) * (Multiplier / 100), MaxHealth);
}

float UBattlerDataComponent::GetMana(float& MaxMana, float Multiplier)
{
	MaxMana = GetModifiedParameter(ManaModifiers, Mana, ManaMultiplier) * (Multiplier / 100);
	return FMath::Min(GetModifiedParameter(ManaModifiers, CurrentMana, ManaMultiplier) * (Multiplier / 100), MaxMana);
}

float UBattlerDataComponent::GetForce(float Multiplier) const
{
	return GetModifiedParameter(ForceModifiers, Force, ForceMultiplier) * (Multiplier / 100);
}

float UBattlerDataComponent::GetResistence(float Multiplier) const
{
	return GetModifiedParameter(ResistenceModifiers, Resistence, ResistenceMultiplier) * (Multiplier / 100);
}

int32 UBattlerDataComponent::GetSpeed(float Multiplier) const
{
	return GetModifiedParameter(SpeedModifiers, Speed, SpeedMultiplier) * (Multiplier / 100);
}

int32 UBattlerDataComponent::GetAdditionalActions() const
{
	return AdditionalActions;
}

int32 UBattlerDataComponent::AddModifier(TArray<float>& Modifiers, float ModifierToAdd)
{
	if (ModifierToAdd < 0.f) return -1;
	return Modifiers.Add(ModifierToAdd);
}

bool UBattlerDataComponent::UpdateModifier(TArray<float>& Modifiers, float ModifierToSearch, float NewModifier)
{
	if (ModifierToSearch < 0.f || NewModifier < 0.f) return false;
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
	if (Index < 0 || NewModifier < 0.f) return false;
	if (Modifiers.IsValidIndex(Index))
	{
		Modifiers[Index] = NewModifier;
		return true;
	}
	return false;
}

bool UBattlerDataComponent::RemoveModifier(TArray<float>& Modifiers, float ModifierToRemove)
{
	if (ModifierToRemove < 0.f) return false;
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
	FinalDamage = GetBattlerOwner()->OnDamageReceived(FinalDamage, bCanBeMitigated);
	GetBattlerOwner()->IndicateDamage(FinalDamage);

	float RemainingDamage = FMath::Max((FinalDamage - CurrentHealth), 0.f);
	CurrentHealth = FMath::Clamp(CurrentHealth - FinalDamage, 0.f, Health);
	float LocalMaxHealth;
	float LocalCurrentHealth = GetHealth(LocalMaxHealth);
	if (LocalCurrentHealth - RemainingDamage <= 0)
	{
		CurrentHealth = 0;
		Kill();
	}
	return FinalDamage;
}

float UBattlerDataComponent::ReceiveHeal(float Heal)
{
	if (Heal < 0) Heal *= -1;
	float LocalMaxHealth;
	float LocalCurrentHealth = GetHealth(LocalMaxHealth);
	float HealedAmount = FMath::Clamp(Heal * (HealMultiplier / 100), 0.f, (LocalMaxHealth - LocalCurrentHealth));
	CurrentHealth += HealedAmount;
	GetBattlerOwner()->IndicateDamage(HealedAmount, true);
	return HealedAmount;
}

float UBattlerDataComponent::ReceiveManaConsume(float Consume)
{
	if (Consume < CurrentMana || Consume == 0) return CurrentMana;
	if (Consume < 0) Consume *= -1;
	CurrentMana = FMath::Clamp(CurrentMana - Consume, 0.f, Mana);
	return CurrentMana;
}

float UBattlerDataComponent::ReceiveManaRestore(float Restore)
{
	if (Restore < 0) Restore *= -1;
	float LocalMaxMana;
	float LocalCurrentMana = GetMana(LocalMaxMana);
	float RestoredAmount = FMath::Clamp(Restore * (ManaRestoreMultiplier / 100), 0.f, (LocalMaxMana - LocalCurrentMana));
	CurrentMana += RestoredAmount;
	return RestoredAmount;
}

void UBattlerDataComponent::Kill()
{
	bIsDead = true;
	GetBattlerOwner()->OnKilled();
	if (CurrentLives > 0)
	{
		CurrentLives--;
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				Revive();
			}, 0.5f, false);
	}
}

void UBattlerDataComponent::Revive(float HealthRestore, float ManaRestore)
{
	if (IsDead())
	{
		bIsDead = false;
		CurrentHealth = Health * (HealthRestore / 100);
		CurrentMana = Mana * (ManaRestore / 100);
	}
	GetBattlerOwner()->OnRevived();
}

bool UBattlerDataComponent::IsDead() const
{
	return bIsDead;
}

