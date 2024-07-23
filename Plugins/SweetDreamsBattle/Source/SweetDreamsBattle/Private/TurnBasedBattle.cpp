// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedBattle.h"
#include "Kismet/GameplayStatics.h"

ATurnBasedBattle::ATurnBasedBattle()
{
	PlayerRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Players"));
	PlayerRoot->SetupAttachment(BattleRoot);

	EnemyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Enemies"));
	EnemyRoot->SetupAttachment(BattleRoot);
}

void ATurnBasedBattle::BeginPlay()
{
	Super::BeginPlay();
	if (BattleWidget)
	{
		TurnBattleWidget = Cast<UTurnBasedBattleWidget>(BattleWidget);
		TurnBattleWidget->SetBattleManager(this);
	}
}

void ATurnBasedBattle::StartBattle(FName State, float BlendTime)
{
	Super::StartBattle(State, BlendTime);
	InitializeTurn();
}

void ATurnBasedBattle::LoadBattlers()
{
	LoadSpawnBattlers(EnemyClasses, Enemies, EnemyTransforms);
	LoadSpawnBattlers(AllyClasses, Allies, AllyTransforms);
}

void ATurnBasedBattle::LoadSpawnBattlers(TArray<TSoftClassPtr<ABattleCharacter>> Battlers, TArray<ABattleCharacter*>& BattlerGroup, TArray<FTransform> TransformGroup)
{
	if (Battlers.Num() == 0)
	{
		return;
	}
	for (int32 i = 0; i < Battlers.Num(); i++)
	{
		TSubclassOf<ABattleCharacter> LoadedBattler = Battlers[i].LoadSynchronous();
		if (LoadedBattler)
		{
			ABattleCharacter* SpawnedBattler = GetWorld()->SpawnActor<ABattleCharacter>(LoadedBattler, TransformGroup[i]);
			if (SpawnedBattler)
			{
				FText NewName = SpawnedBattler->GetCharacterName();
				FText IndexText = FText::AsNumber(i);
				FText CombinedText = FText::Format(FText::FromString("{0}{1}"), NewName, IndexText);
				SpawnedBattler->SetActorLabel(CombinedText.ToString());
				BattlerGroup.Add(SpawnedBattler);
			}
		}
	}
	if (Allies.Num() > 0)
	{
		AllBattlers.Append(Allies);
	}
	if (Enemies.Num() > 0)
	{
		AllBattlers.Append(Enemies);
	}
}

void ATurnBasedBattle::ChangeCameraView(ECameraFocus NewFocus, float BlendTime)
{
	if (NewFocus == ECameraFocus::Self)
	{
		// AllyInput->MulticameraComponent ...
		ChangeCameraFocus(AllyInput, BlendTime);
		return;
	}
	Super::ChangeCameraView(NewFocus, BlendTime);
}

void ATurnBasedBattle::InitializeTurn() // resets actions to initialize turn
{
	CurrentAction = -1;
	CurrentAllyIndex = 0;
	for (ABattleCharacter* Battler : AllBattlers)
	{
		Battler->UpdateActionsCooldown();
	}
	Actions.Empty();
	if (EvaluateEndBattle())
	{
		return;
	}
	GetEnemyActions();
	FTimerHandle LocalHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("AllyInputStart"), nullptr);
	GetWorldTimerManager().SetTimer(LocalHandle, TimerDel, CameraDelay, false);
}

void ATurnBasedBattle::GetEnemyActions()
{
	for (ABattleCharacter * Enemy : Enemies)
	{
		if (Enemy->GetBattlerParameters()->IsDead() || Enemy->IsPendingKill())
		{
			continue;
		}
		UBattleAction* Action = Enemy->GetRandomAction();
		if (Action)
		{
			Action->SetBattle(this);
			if (Allies.Num() > 0)
			{
				Action->SetTargetRandom(Allies, Action->GetTargetAmount());
			}
			Actions.Add(Action);
		}
		// get speed and convert into ActionOrder.Add(num)
	}
}

void ATurnBasedBattle::AllyInputStart(ABattleCharacter* Ally)
{
	int32 LocalIndex = 0;
	if (!Ally && Allies.Num() > 0)
	{
		Ally = Allies[LocalIndex];
	}
	AllyInput = Ally;
	if (!AllyInput) return;
	while (AllyInput->GetBattlerParameters()->IsDead() || AllyInput->IsPendingKill())
	{
		LocalIndex++;
		if (LocalIndex >= Allies.Num())
		{
			bIsVictorious = false;
			EndBattle();
			return;
		}
		AllyInput = Allies[LocalIndex];
		if (!AllyInput) return;
	}
	if (Player)
	{
		ChangeCameraFocusDelayed(AllyInput, BattlerBlendTime, CameraDelay);
	}
	if (TurnBattleWidget)
	{
		TurnBattleWidget->ShowPlayerInput(); // make it delayed along with the BattlerBlendTime.
	}
}

void ATurnBasedBattle::AllySelectAction(UBattleAction* Action)
{
	if (!Action)
	{
		return;
	}
	AllySelectedAction = Action;
	UpdatePossibleTargets(Action);
	ECameraFocus NewFocus;
	ETargetType TargetType = Action->GetTargetType();
	switch (TargetType)
	{
	case ETargetType::Ally:
	case ETargetType::DeadAlly:
		NewFocus = ECameraFocus::Allies;
		break;
	case ETargetType::Enemy:
		NewFocus = ECameraFocus::Enemies;
		break;
	case ETargetType::Self:
		NewFocus = ECameraFocus::Self;
		break;
	default:
		NewFocus = ECameraFocus::AllBattlers;
		break;
	}
	ChangeCameraView(NewFocus, BattlerBlendTime);
}

void ATurnBasedBattle::AllySelectTargets(TArray<ABattleCharacter*> Targets)
{
	if (Targets.Num() == 0)
	{
		return;
	}
	AllyTargets = Targets;
}

void ATurnBasedBattle::AllyActionConfirm()
{
	CurrentAllyIndex++;
	CurrentAllyIndex = FMath::Clamp(CurrentAllyIndex, 0, Allies.Num() - 1);
	if (AllySelectedAction)
	{
		AllySelectedAction->SetBattle(this);
		AllySelectedAction->SetTarget(AllyTargets);
		Actions.Add(AllySelectedAction);
		// get speed and convert into ActionOrder.Add(num)
	}
	EvaluateTurnStart();
}

void ATurnBasedBattle::AddActionAuto(UBattleAction* Action, bool bAddAsLast)
{
	if (Action)
	{
		if (bAddAsLast)
		{
			Actions.Add(Action);
		}
		else
		{
			// get speed and convert into ActionOrder.Add(num)
		}
	}
}

void ATurnBasedBattle::UpdatePossibleTargets(UBattleAction* Action)
{
	TArray<ABattleCharacter*> NewTargets;
	ETargetType TargetType = Action->GetTargetType();
	switch (TargetType)
	{
	case ETargetType::Ally:
		NewTargets = Allies;
		if (!Action->GetIfIncludeSelf())
		{
			NewTargets.Remove(AllyInput);
		}
		break;
	case ETargetType::DeadAlly:
		NewTargets = Allies;
		for (ABattleCharacter* Ally : NewTargets)
		{
			if (Ally->GetBattlerParameters()->IsDead())
			{
				NewTargets.Remove(Ally);
			}
		}
		break;
	case ETargetType::Enemy:
		NewTargets = Enemies;
		break;
	case ETargetType::Self:
		NewTargets.Add(AllyInput);
		break;
	default:
		break;
	}
	AllyPossibleTargets = NewTargets;
}

ABattleCharacter* ATurnBasedBattle::GetCurrentInputAlly() const
{
	return AllyInput;
}

TArray<ABattleCharacter*> ATurnBasedBattle::GetPossibleTargets() const
{
	return AllyPossibleTargets;
}

void ATurnBasedBattle::EvaluateTurnStart() // checks if all allies declared action to start the turn with the first action.
{
	if (Allies.Num() == Actions.Num() - Enemies.Num())
	{
		if (Player)
		{
			ChangeCameraFocus(this, BattlerBlendTime);
		}
		StartActionInOrder();
		if (TurnBattleWidget)
		{
			TurnBattleWidget->HidePlayerInput();
		}
	}
	else
	{
		AllyInputStart(Allies[CurrentAllyIndex]);
	}
}

void ATurnBasedBattle::StartActionInOrder() // called when Action ends.
{
	if (EvaluateEndBattle())
	{
		return;
	}
	CurrentAction = FMath::Clamp(CurrentAction + 1, 0, Actions.Num());
	if (CurrentAction >= Actions.Num())
	{
		if (Player)
		{
			ChangeCameraFocus(this, BattlerBlendTime);
		}
		InitializeTurn();
		return;
	}
	UBattleAction* CurrentActionRef = Actions[CurrentAction]; //Actions[ActionOrder[CurrentAction]]->OnActionStart();
	if (!CurrentActionRef) return;
	while (CurrentActionRef->GetOwner()->GetBattlerParameters()->IsDead() || CurrentActionRef->GetOwner()->IsPendingKill())
	{
		CurrentAction++;
		if (CurrentAction >= Actions.Num())
		{
			if (Player)
			{
				ChangeCameraFocus(this, BattlerBlendTime);
			}
			InitializeTurn();
			return;
		}
		CurrentActionRef = Actions[CurrentAction]; //Actions[ActionOrder[CurrentAction]]->OnActionStart();
		if (!CurrentActionRef) return;
	}
	ChangeCameraFocus(CurrentActionRef->GetOwner(), BattlerBlendTime);
	FTimerHandle LocalHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(CurrentActionRef, FName("StartAction"), true);
	GetWorldTimerManager().SetTimer(LocalHandle, TimerDel, BattlerBlendTime + ActionDelay, false);
}

void ATurnBasedBattle::EndBattle(FName State, float BlendTime)
{
	CurrentAction = -1;
	CurrentAllyIndex = 0;
	for (ABattleCharacter* Battler : AllBattlers)
	{
		Battler->ResetActions();
	}
	Actions.Empty();
	Super::EndBattle(State, BlendTime);
}


