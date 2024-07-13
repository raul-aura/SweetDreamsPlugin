// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsBattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "SweetDreamsGameMode.h"

ASweetDreamsBattleManager::ASweetDreamsBattleManager()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* BattleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Battle Manager"));
	BattleRoot->SetupAttachment(RootComponent);

	PlayerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Players"));
	PlayerRoot->SetupAttachment(BattleRoot);

	EnemyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Enemies"));
	EnemyRoot->SetupAttachment(BattleRoot);

	BattleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Battle Camera"));
	BattleCamera->SetupAttachment(BattleRoot);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);
}

void ASweetDreamsBattleManager::BeginPlay()
{
	Super::BeginPlay();
	MulticameraComponent->DefinePrimaryCamera(BattleCamera);
	PlayerRoot->GetChildrenComponents(true, PlayerLocations);
	EnemyRoot->GetChildrenComponents(true, EnemiesLocations);
	BattleWidget = CreateWidget(this, BattleWidgetClass);
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Collapsed);
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

void ASweetDreamsBattleManager::StartBattle(FName State, float blendTime)
{
	IsBattleActive = true;
	ASweetDreamsGameMode* GameMode = Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->StartState(GameMode->GetStateByName(State));
	}
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0))
	{
		Player->StopMovement();
		Player->SetViewTargetWithBlend(this, blendTime);
	}
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASweetDreamsBattleManager::EndBattle(FName State, float blendTime)
{
	IsBattleActive = false;
	ASweetDreamsGameMode* GameMode = Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->StartState(GameMode->GetStateByName(State));
	}
	if (APlayerController* Player = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			Player->SetViewTargetWithBlend(PlayerPawn, blendTime);
		}
	}
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

