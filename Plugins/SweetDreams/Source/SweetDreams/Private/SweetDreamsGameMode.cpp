// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsGameMode.h"
#include "SweetDreamsBPLibrary.h"
#include "SweetDreamsCharacter.h"
#include "SweetDreamsSettings.h"
#include "SweetDreamsPlayerController.h"

ASweetDreamsGameMode::ASweetDreamsGameMode()
{
	DefaultPawnClass = ASweetDreamsCharacter::StaticClass();
	PlayerControllerClass = ASweetDreamsPlayerController::StaticClass();
}

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
			CreateAddState(State);
		}
		if (DreamStates.Num() > 0)
		{
			if (Core->CoreSettings->bOverrideInitialState)
			{
				ASweetDreamsState* NewState = GetStateByName(Core->CoreSettings->NewInitialState);
				StartState(NewState);
				if (Core->CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintStateOperations))
				{
					USweetDreamsBPLibrary::PrintDream(this, FString::Printf(TEXT("Overriding initial state with %s."), *NewState->GetStateName().ToString()));
				}
			}
			else
			{
				StartState(GetStateByIndex(0));
			}
		}
	}
	LoadingWidget = CreateLoadingWidget(LoadingWidgetClass);
	Super::BeginPlay();
}

ASweetDreamsState* ASweetDreamsGameMode::GetStateByName(FName StateName) const
{
	if (DreamStates.Num() > 0)
	{
		for (ASweetDreamsState* State : DreamStates)
		{
			if (State->GetStateName() == StateName)
			{
				return State;
			}
		}
		if (Core->CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintStateOperations))
		{
			USweetDreamsBPLibrary::PrintDream(this, FString::Printf(TEXT("State with name %s NOT FOUND, returning State on index 0 instead."), *StateName.ToString()), EPrintType::WARNING);
		}
		return DreamStates[0];
	}
	return nullptr;
}

ASweetDreamsState* ASweetDreamsGameMode::GetStateByIndex(int32 Index) const
{
	if (DreamStates.Num() > 0 && Index >= 0)
	{
		return (DreamStates[Index]) ? DreamStates[Index] : DreamStates[0];
	}
	return nullptr;
}

void ASweetDreamsGameMode::StartState(ASweetDreamsState* State)
{
	if (!State) return;
	if (CurrentState)
	{
		CurrentState->EndState();
	}
	CurrentState = State;
	CurrentState->StartState();
}

bool ASweetDreamsGameMode::CreateAddState(TSubclassOf<ASweetDreamsState> StateClass)
{
	if (!StateClass) return false;
	ASweetDreamsState* NewState = GetWorld()->SpawnActor<ASweetDreamsState>(StateClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (NewState)
	{
		DreamStates.Add(NewState);
		if (Core->CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintStateOperations))
		{
			if (Core->CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintStateCreated))
			{
				USweetDreamsBPLibrary::PrintDream(this, FString::Printf(TEXT("%s State CREATED."), *NewState->GetStateName().ToString()));
			}
		}

		return true;
	}
	if (Core->CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintStateOperations))
	{
		if (Core->CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintStateCreated))
		{
			USweetDreamsBPLibrary::PrintDream(this, FString::Printf(TEXT("%s State FAILED to create."), *NewState->GetStateName().ToString()), EPrintType::ERROR);
		}
	}
	return false;
}

ULoadingWidget* ASweetDreamsGameMode::CreateLoadingWidget(TSubclassOf<ULoadingWidget> Class)
{
	if (!Class) return nullptr;
	ULoadingWidget* NewWidget = CreateWidget<ULoadingWidget>(GetWorld(), Class);
	if (NewWidget)
	{
		NewWidget->AddToViewport(99);
		FTimerHandle WidgetRemoveTimer;
		GetWorld()->GetTimerManager().SetTimer(WidgetRemoveTimer, [NewWidget]()
			{
				NewWidget->SetVisibility(ESlateVisibility::Collapsed);
			}, LoadingDelay, false);
	}
	return NewWidget;
}


