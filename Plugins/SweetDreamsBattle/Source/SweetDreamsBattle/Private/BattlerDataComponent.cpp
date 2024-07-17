// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlerDataComponent.h"

UBattlerDataComponent::UBattlerDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
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

float UBattlerDataComponent::GetHealth() const
{
	return Health * GetModifiers(HealthModifiers, HealthMultiplier);
}

float UBattlerDataComponent::GetForce() const
{
	return Force * GetModifiers(ForceModifiers, ForceMultiplier);
}

float UBattlerDataComponent::GetResistence() const
{
	return Resistence * GetModifiers(ResistenceModifiers, ResistenceMultiplier);
}

