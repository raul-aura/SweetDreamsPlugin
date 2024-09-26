// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsState.h"
#include "Kismet/GameplayStatics.h"

void ASweetDreamsState::BeginPlay()
{
	World = GetWorld();
	Player = UGameplayStatics::GetPlayerController(this, 0);
	if (World)
	{
		CoreState = World->GetGameInstance()->GetSubsystem<USweetDreamsCore>();
	}
	Super::BeginPlay();
}

void ASweetDreamsState::StartState_Implementation() 
{
	if (CoreState)
	{
		if (CoreState->CoreSettings->DebugFlags & static_cast<uint8>(EDebugFlags::PrintStateOperations))
		{
			CoreState->PrintDream(this, FString::Printf(TEXT("%s State INITIALIZED."), *GetStateName().ToString()));
		}
	}
}

void ASweetDreamsState::EndState_Implementation() {}

FName ASweetDreamsState::GetStateName() const
{
	return StateName;
}

// STATE AWAKE
ADreamStateAwake::ADreamStateAwake()
{
	StateName = "Awake";
}

void ADreamStateAwake::StartState_Implementation()
{
	Super::StartState_Implementation();
	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(1.0f);
	}
	if (Player)
	{
		EnableInput(Player);
		Player->SetShowMouseCursor(false);
		FInputModeGameOnly ModeGame;
		Player->SetInputMode(ModeGame);
	}
}

// STATE AWAKE_UI
ADreamStateAwakeUI::ADreamStateAwakeUI()
{
	StateName = "AwakeUI";
}

void ADreamStateAwakeUI::StartState_Implementation()
{
	Super::StartState_Implementation();
	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(1.0f);
	}
	if (Player)
	{
		Player->SetShowMouseCursor(true);
		FInputModeGameAndUI ModeGameUi;
		Player->SetInputMode(ModeGameUi);
		Player->StopMovement();
		DisableInput(Player);
	}
}

// STATE ASLEEP
ADreamStateAsleep::ADreamStateAsleep()
{
	StateName = "Asleep";
}

void ADreamStateAsleep::StartState_Implementation()
{
	Super::StartState_Implementation();
	if (World)
	{
		World->GetWorldSettings()->SetTimeDilation(0.0f);
	}
	if (Player)
	{
		Player->SetShowMouseCursor(true);
		FInputModeUIOnly ModeUI;
		Player->SetInputMode(ModeUI);
		DisableInput(Player);
	}
}