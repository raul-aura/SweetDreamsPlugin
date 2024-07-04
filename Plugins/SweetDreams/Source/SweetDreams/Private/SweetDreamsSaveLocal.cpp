// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsSaveLocal.h"

void USweetDreamsSaveLocal::OnSaveLoaded(TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		ISweetDreamsSaveInterface::Execute_OnLocalLoaded(Actor, this);
	}
}

void USweetDreamsSaveLocal::OnSaveSaved(TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		ISweetDreamsSaveInterface::Execute_OnLocalSaved(Actor, this);
	}
}