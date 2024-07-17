// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsBattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "SweetDreamsGameMode.h"

ASweetDreamsBattleManager::ASweetDreamsBattleManager()
{
	PrimaryActorTick.bCanEverTick = true;
	BattleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Battle Manager"));
	BattleRoot->SetupAttachment(RootComponent);

	BattleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Battle Camera"));
	BattleCamera->SetupAttachment(BattleRoot);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);
}

void ASweetDreamsBattleManager::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		BattleWidget = CreateWidget<UUserWidget>(World, BattleWidgetClass);
	}
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Collapsed);
		BattleWidget->AddToViewport();
	}
}

void ASweetDreamsBattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASweetDreamsBattleManager::IsBattleOngoing() const
{
	return IsBattleActive;
}

void ASweetDreamsBattleManager::StartBattle(FName State, float BlendTime)
{
	IsBattleActive = true;
	ASweetDreamsGameMode* GameMode = Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode && State != "None")
	{
		GameMode->StartState(GameMode->GetStateByName(State));
	}
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0))
	{
		Player->StopMovement();
		Player->SetViewTargetWithBlend(this, BlendTime);
	}
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASweetDreamsBattleManager::EndBattle(FName State, float BlendTime)
{
	IsBattleActive = false;
	ASweetDreamsGameMode* GameMode = Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode && State != "None")
	{
		GameMode->StartState(GameMode->GetStateByName(State));
	}
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			Player->SetViewTargetWithBlend(PlayerPawn, BlendTime);
		}
	}
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASweetDreamsBattleManager::EvaluateEndBattle() {}

