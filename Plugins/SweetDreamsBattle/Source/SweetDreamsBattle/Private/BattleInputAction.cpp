// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleInputAction.h"
#include "UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "TurnBasedBattleWidget.h"

UBattleInputAction::UBattleInputAction()
{
	ElementName = FText::FromString(TEXT("Input"));
	bOverrideOwnerSpeed = true;
	ActionSpeed = 999;
	Cost = 0;
}

void UBattleInputAction::StartAction(bool bUseCooldown)
{
	if (!InputWidgetClass) return;
	if (!LoadWidget()) return;
	Super::StartAction(false);
}

bool UBattleInputAction::LoadWidget()
{
	TArray<UUserWidget*> AllWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, AllWidgets, InputWidgetClass, true);
	if (AllWidgets.Num() > 0)
	{
		UUserWidget* LoadedWidget = AllWidgets[0];
		if (LoadedWidget)
		{
			InputWidget = LoadedWidget;
			return true;
		}
	}
	return false;
}
