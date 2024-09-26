// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleNumberWidget.h"
#include "BattleCharacter.h"

void UBattleNumberWidget::RemoveThisIndicator()
{
	OwningBattler->RemoveDamageIndicator(OwningWidgetComponent);
}
