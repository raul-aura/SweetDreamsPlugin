// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsBattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
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
	Player = UGameplayStatics::GetPlayerController(this, 0);
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

void ASweetDreamsBattleManager::StartBattle(FName State, float BlendTime)
{
	if (bIsBattleActive)
	{
		return;
	}
	LoadBattlers();
	bIsBattleActive = true;
	ASweetDreamsGameMode* GameMode = Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode && State != "None")
	{
		GameMode->StartState(GameMode->GetStateByName(State));
	}
	if (Player)
	{
		Player->StopMovement();
		ChangeCameraFocus(this, BlendTime);
	}
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Visible);
	}
	OnBattleStart();
}

void ASweetDreamsBattleManager::LoadBattlers() {}

void ASweetDreamsBattleManager::EndBattle(FName State, float BlendTime)
{
	bIsBattleActive = false;
	ASweetDreamsGameMode* GameMode = Cast<ASweetDreamsGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode && State != "None")
	{
		GameMode->StartState(GameMode->GetStateByName(State));
	}
	if (APawn* PlayerPawn = Player->GetPawn())
	{
		ChangeCameraFocus(PlayerPawn, BlendTime);
	}
	if (BattleWidget)
	{
		BattleWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	OnBattleEnd(bIsVictorious);
}

bool ASweetDreamsBattleManager::EvaluateEndBattle()
{ 
	if (Enemies.Num() > 0)
	{
		for (ABattleCharacter* Enemy : Enemies)
		{
			if (!Enemy->GetBattlerParameters()->IsDead())
			{
				return false;
			}
		}
	}
	bIsVictorious = true;
	EndBattle();
	return bIsVictorious;
}

bool ASweetDreamsBattleManager::IsBattleOngoing() const
{
	return bIsBattleActive;
}

bool ASweetDreamsBattleManager::IsBattleVictorious() const
{
	return bIsVictorious;
}

void ASweetDreamsBattleManager::ChangeCameraFocus(AActor* NewFocus, float BlendTime)
{
	if (!Player)
	{
		return;
	}
	if (BlendTime < 0.0f)
	{
		BlendTime = BattlerBlendTime;
	}
	Player->SetViewTargetWithBlend(NewFocus, BlendTime);
}

void ASweetDreamsBattleManager::ChangeCameraFocusDelayed(AActor* NewFocus, float BlendTime, float DelayTime)
{
	if (DelayTime <= 0)
	{
		DelayTime = GetWorld()->GetDeltaSeconds();
	}
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("ChangeCameraFocus"), NewFocus, BlendTime);
	GetWorldTimerManager().SetTimer(BattleTimer, TimerDel, DelayTime, false);
}

void ASweetDreamsBattleManager::ChangeCameraView(ECameraFocus NewFocus, float BlendTime)
{
	ChangeCameraFocus(this, BattlerBlendTime);
	int32 Index = static_cast<int32>(NewFocus);
	if (Index <= MulticameraComponent->GetAllPossibleViews().Num() - 1)
	{
		MulticameraComponent->SetNewCameraView(Index, BlendTime);
	}
}

