// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlerDataComponent.h"

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

float UBattlerDataComponent::GetModifiers(TArray<float> Modifiers, float BaseMultiplier) const
{
	float Multiplier = 0;
	Modifiers.Add(BaseMultiplier);
	for (float Modifier : Modifiers)
	{
		Multiplier += Modifier;
	}
	Multiplier = FMath::Max(Multiplier, 0.0f);
	return Multiplier / 100;
}

ABattleCharacter* UBattlerDataComponent::GetBattlerOwner() const
{
	return Cast<ABattleCharacter>(GetOwner());
}

float UBattlerDataComponent::GetHealth() const
{
	return CurrentHealth * GetModifiers(HealthModifiers, HealthMultiplier);
}

float UBattlerDataComponent::GetForce() const
{
	return Force * GetModifiers(ForceModifiers, ForceMultiplier);
}

float UBattlerDataComponent::GetResistence() const
{
	return Resistence * GetModifiers(ResistenceModifiers, ResistenceMultiplier);
}

float UBattlerDataComponent::ReceiveDamage(float Damage, bool bCanBeMitigated)
{
	float FinalDamage = Damage;
	if (bCanBeMitigated)
	{
		GetMitigatedDamage(FinalDamage);
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
	float HealedAmount = FMath::Clamp(Heal, 0.0f, (Health - CurrentHealth));
	CurrentHealth += HealedAmount;
	return HealedAmount;
}

void UBattlerDataComponent::GetMitigatedDamage_Implementation(float& Mitigated)
{
	Mitigated -= GetResistence();
	Mitigated = FMath::Max(Mitigated, 0.0f);
}

void UBattlerDataComponent::Kill_Implementation()
{
	bIsDead = true;
}

void UBattlerDataComponent::Revive_Implementation()
{
	if (IsDead())
	{
		bIsDead = false;
	}
}

bool UBattlerDataComponent::IsDead() const
{
	return bIsDead;
}

