// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleElement.h"

void UBattleElement::SetOwner(ABattleCharacter* InputOwner)
{
	Owner = InputOwner;
}

void UBattleElement::SetBattle(ASweetDreamsBattleManager* Battle)
{
	CurrentBattle = Battle;
}

void UBattleElement::AddTarget(ABattleCharacter* Target)
{
	Targets.Add(Target);
}

void UBattleElement::SetTarget(TArray<ABattleCharacter*> NewTargets)
{
	Targets = NewTargets;
}
