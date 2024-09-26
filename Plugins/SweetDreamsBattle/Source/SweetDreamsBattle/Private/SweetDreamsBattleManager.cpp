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

	DamageIndicatorClass = UBattleNumberWidget::StaticClass();
}

void ASweetDreamsBattleManager::BeginPlay()
{
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
	Super::BeginPlay();
}

void ASweetDreamsBattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASweetDreamsBattleManager::StartBattleByIndex(const UObject* WorldContext, int32 Index)
{
	ASweetDreamsBattleManager* Battle = FindBattleByIndex(WorldContext, Index);
	if (Battle)
	{
		Battle->StartBattle();
	}
}

ASweetDreamsBattleManager* ASweetDreamsBattleManager::FindBattleByIndex(const UObject* WorldContext, int32 Index)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")) || Index < 0)
	{
		return nullptr;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsBattleManager::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASweetDreamsBattleManager* BattleManager = Cast<ASweetDreamsBattleManager>(Actor);
		if (BattleManager && BattleManager->BattleIndex == Index)
		{
			return BattleManager;
		}
	}
	return nullptr;
}

ASweetDreamsBattleManager* ASweetDreamsBattleManager::FindActiveBattle(const UObject* WorldContext, int32& BattleId)
{
	BattleId = -1;
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsBattleManager::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASweetDreamsBattleManager* BattleManager = Cast<ASweetDreamsBattleManager>(Actor);
		if (BattleManager && BattleManager->bIsBattleActive)
		{
			BattleId = BattleManager->BattleIndex;
			return BattleManager;
		}
	}
	return nullptr;
}

void ASweetDreamsBattleManager::StartBattle(float BlendTime)
{
	if (bIsBattleActive) return;
	LoadBattlers();
	bIsBattleActive = true;
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

void ASweetDreamsBattleManager::LoadBattlers_Implementation()
{
	EnemyDamage.Init(0.0f, Enemies.Num());
	AllyDamage.Init(0.0f, Allies.Num());
}

void ASweetDreamsBattleManager::EndBattle(float BlendTime)
{
	bIsBattleActive = false;
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

bool ASweetDreamsBattleManager::EvaluateEndBattle_Implementation()
{ 
	if (!bIsBattleActive) return false;
	bool bAllEnemiesDead = true;
	bool bAllAlliesDead = true;
	if (Enemies.Num() > 0)
	{
		for (ABattleCharacter* Enemy : Enemies)
		{
			if (!Enemy->GetBattlerParameters()->IsDead())
			{
				bAllEnemiesDead = false;
				break;
			}
		}
	}
	if (Allies.Num() > 0)
	{
		for (ABattleCharacter* Ally : Allies)
		{
			if (!Ally->GetBattlerParameters()->IsDead())
			{
				bAllAlliesDead = false;
				break;
			}
		}
	}
	if (bAllEnemiesDead || bAllAlliesDead)
	{
		if (bAllEnemiesDead)
		{
			bIsVictorious = true;
		}
		else
		{
			bIsVictorious = false;
		}
		EndBattle();
		return true;
	}
	return false;
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
	if (!Player || !NewFocus) return;
	if (BlendTime < 0.0f)
	{
		BlendTime = BattlerBlendTime;
	}
	Player->SetViewTargetWithBlend(NewFocus, BlendTime);
}

void ASweetDreamsBattleManager::ChangeCameraView(ECameraView NewView, AActor* SelfFocus, float BlendTime)
{
	int32 Index = static_cast<int32>(NewView);
	if (NewView == ECameraView::Self)
	{
		Index = 1;
	}
	else if ((NewView != ECameraView::Self) || !SelfFocus)
	{
		SelfFocus = this;
	}
	ChangeCameraFocus(SelfFocus, BlendTime);
	UMulticameraComponent* Multicamera = SelfFocus->FindComponentByClass<UMulticameraComponent>();
	if (Multicamera && Index <= Multicamera->GetAllPossibleViews().Num() - 1)
	{
		Multicamera->SetNewCameraView(Index, BlendTime);
	}
}

void ASweetDreamsBattleManager::AddDamageToBattle(ABattleCharacter* DamageOwner, float Damage, bool bApplyCalculations)
{
	if (!DamageOwner || Damage <= 0.0f) return;
	int32 Index = Enemies.Find(DamageOwner);
	if (Index != INDEX_NONE)
	{
		EnemyDamage[Index] += Damage;
		OnDamageApplied(DamageOwner, Damage, false, bApplyCalculations);
		return;
	}
	Index = Allies.Find(DamageOwner);
	if (Index != INDEX_NONE)
	{
		AllyDamage[Index] += Damage;
	}
	OnDamageApplied(DamageOwner, Damage, true, bApplyCalculations);
}

float ASweetDreamsBattleManager::GetAllAlliedDamage() const
{
	float Damage = 0.f;
	if (AllyDamage.Num() > 0)
	{
		for (float Instance : AllyDamage)
		{
			Damage += Instance;
		}
	}
	return Damage;
}

float ASweetDreamsBattleManager::GetAllEnemyDamage() const
{
	float Damage = 0.f;
	if (EnemyDamage.Num() > 0)
	{
		for (float Instance : EnemyDamage)
		{
			Damage += Instance;
		}
	}
	return Damage;
}

