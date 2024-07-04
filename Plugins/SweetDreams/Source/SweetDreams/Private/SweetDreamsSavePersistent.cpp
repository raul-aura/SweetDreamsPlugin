// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsSavePersistent.h"

void USweetDreamsSavePersistent::OnSaveLoaded(TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		ISweetDreamsSaveInterface::Execute_OnPersistentLoaded(Actor, this);
	}
}

void USweetDreamsSavePersistent::OnSaveSaved(TArray<AActor*>& Actors)
{
	for (AActor* Actor : Actors)
	{
		ISweetDreamsSaveInterface::Execute_OnPersistentSaved(Actor, this);
	}
}
