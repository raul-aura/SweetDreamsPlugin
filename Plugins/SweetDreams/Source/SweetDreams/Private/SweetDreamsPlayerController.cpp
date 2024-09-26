// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsPlayerController.h"

ASweetDreamsCharacter* ASweetDreamsPlayerController::GetDreamCharacter() const
{
	return Cast<ASweetDreamsCharacter>(GetCharacter());
}

void ASweetDreamsPlayerController::DisableInputTimer(float Duration)
{
	FTimerHandle InputTimer;
	DisableInput(this);
	GetWorldTimerManager().SetTimer(InputTimer, [this]() {
		EnableInput(this);
		}, Duration, false);
}
