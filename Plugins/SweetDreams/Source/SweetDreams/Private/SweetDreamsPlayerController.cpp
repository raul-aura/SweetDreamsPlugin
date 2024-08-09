// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsPlayerController.h"

void ASweetDreamsPlayerController::SetViewTargetWithBlend(AActor* NewViewTarget, float BlendTime, EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	Super::SetViewTargetWithBlend(NewViewTarget, BlendTime, BlendFunc, BlendExp, bLockOutgoing);
}

ASweetDreamsCharacter* ASweetDreamsPlayerController::GetDreamCharacter() const
{
	return Cast<ASweetDreamsCharacter>(GetCharacter());
}