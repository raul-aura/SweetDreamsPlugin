// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsGameMode.h"
#include "SweetDreamsBPLibrary.h"

void ASweetDreamsGameMode::BeginPlay()
{
	Core = GetGameInstance()->GetSubsystem<USweetDreamsCore>();
	if (Core && Core->CoreSettings)
	{
		if (Core->CoreSettings->bEnableAutoLoadData && Core->CoreSettings->bEnableAutoLoadSave)
		{
			Core->ManageSaveData(false);
			Core->ManageSaveData(false, false);
		}
		TArray<TSubclassOf<ASweetDreamsState>> StateClasses = Core->CoreSettings->DreamStates;
		for (TSubclassOf<ASweetDreamsState>& State : StateClasses)
		{
			ASweetDreamsState* NewState = GetWorld()->SpawnActor<ASweetDreamsState>(State, FVector::ZeroVector, FRotator::ZeroRotator);
			if (NewState)
			{
				DreamStates.Add(NewState);
				USweetDreamsBPLibrary::PrintDream(this, FString::Printf(TEXT("%s State CREATED."), *NewState->GetStateName().ToString()));
			}
			else
			{
				USweetDreamsBPLibrary::PrintDream(this, FString::Printf(TEXT("%s State FAILED to create."), *NewState->GetStateName().ToString()));
			}
		}
		if (DreamStates.Num() != 0)
		{
			if (Core->CoreSettings->bOverrideInitialState)
			{
				StartState(GetStateByName(Core->CoreSettings->NewInitialState));
			}
			else
			{
				StartState(GetStateByIndex(0));
			}
		}
	}
	GetUserSettingsDelegate.Broadcast(Core->GetUserSettings());
	Super::BeginPlay();
}

// STATE

ASweetDreamsState* ASweetDreamsGameMode::GetStateByName(FName StateName) const
{
	if (DreamStates.Num() != 0)
	{
		for (ASweetDreamsState* State : DreamStates)
		{
			if (State->GetStateName() == StateName)
			{
				return State;
			}
		}
		return DreamStates[0];
	}
	return nullptr;
}

ASweetDreamsState* ASweetDreamsGameMode::GetStateByIndex(int32 Index) const
{
	if (DreamStates.Num() != 0 && Index >= 0)
	{
		return (DreamStates[Index]) ? DreamStates[Index] : DreamStates[0];
	}
	return nullptr;
}

void ASweetDreamsGameMode::StartState(ASweetDreamsState* State)
{
	if (!State)
	{
		return;
	}
	if (CurrentState)
	{
		CurrentState->EndState();
	}
	CurrentState = State;
	CurrentState->StartState();
}