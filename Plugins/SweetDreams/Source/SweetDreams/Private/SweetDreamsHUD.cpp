// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsHUD.h"
#include "Kismet/GameplayStatics.h"
#include "SweetDreamsWidget.h"

TArray<USweetDreamsWidget*> ASweetDreamsHUD::AllWidgets;

void ASweetDreamsHUD::BeginPlay()
{
	Super::BeginPlay();
	CreateWidgets();
}

void ASweetDreamsHUD::CreateWidgets()
{
	AllWidgets.Empty();
	for (TSubclassOf<USweetDreamsWidget> Widget : DefaultWidgets)
	{
		USweetDreamsWidget* NewWidget = CreateWidget<USweetDreamsWidget>(GetOwningPlayerController(), Widget);
		if (NewWidget)
		{
			NewWidget->SetVisibility(ESlateVisibility::Collapsed);
			NewWidget->AddToViewport();
			AllWidgets.Add(NewWidget);
		}
	}
}

void ASweetDreamsHUD::CreateAndStoreWidget(TSubclassOf<USweetDreamsWidget> WidgetClass)
{
	if (!WidgetClass) return;
	USweetDreamsWidget* NewWidget = CreateWidget<USweetDreamsWidget>(GetOwningPlayerController(), WidgetClass);
	if (NewWidget)
	{
		NewWidget->SetVisibility(ESlateVisibility::Collapsed);
		NewWidget->AddToViewport();
		AllWidgets.Add(NewWidget);
	}
}

void ASweetDreamsHUD::ShowWidget(USweetDreamsWidget* Widget)
{
	if (!Widget) return;
	Widget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	Widget->OnShow();
	UpdatePlayerInputMode(Widget);
}

void ASweetDreamsHUD::HideWidget(USweetDreamsWidget* Widget)
{
	if (!Widget) return;
	Widget->SetVisibility(ESlateVisibility::Collapsed);
	Widget->OnHide();
	UpdatePlayerInputMode(Widget);
}

USweetDreamsWidget* ASweetDreamsHUD::FindWidgetByClass(TSubclassOf<USweetDreamsWidget> WidgetClass)
{
	if (!WidgetClass || AllWidgets.Num() == 0) return nullptr;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (Widget && Widget->IsA(WidgetClass)) return Widget;
	}
	return nullptr;
}

USweetDreamsWidget* ASweetDreamsHUD::FindWidgetByName(FName WidgetName)
{
	if (WidgetName.IsNone() || AllWidgets.Num() == 0) return nullptr;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (Widget && Widget->WidgetName.IsEqual(WidgetName)) return Widget;
	}
	return nullptr;
}

void ASweetDreamsHUD::UpdatePlayerInputMode(USweetDreamsWidget* WidgetToFocus)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WidgetToFocus, EGetWorldErrorMode::ReturnNull);
	if (!ensureAlwaysMsgf(IsValid(WidgetToFocus), TEXT("World Context was not valid.")))
	{
		return;
	}
	if (APlayerController* Player = World->GetFirstPlayerController())
	{
		if (IsAnyWidgetVisible())
		{
			FInputModeGameAndUI NewMode;
			if (WidgetToFocus) NewMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
			Player->SetInputMode(NewMode);
			Player->SetShowMouseCursor(true);
		}
		else
		{
			Player->SetInputMode(FInputModeGameOnly());
			Player->SetShowMouseCursor(false);
		}
	}
}

bool ASweetDreamsHUD::IsAnyWidgetVisible()
{
	if (AllWidgets.Num() == 0) return false;
	for (USweetDreamsWidget* Widget : AllWidgets)
	{
		if (Widget->IsVisible() && !Widget->bIgnoreThisForVisibility) return true;
	}
	return false;
}


