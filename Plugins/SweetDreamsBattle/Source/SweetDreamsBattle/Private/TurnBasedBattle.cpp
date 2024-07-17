// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedBattle.h"

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
	Super::StartBattle(State, BlendTime); // FOR SOME REASON THE PLAYER DOESNT STOP MOVINGGG!!!!
	LoadSpawnBattlers(EnemyClasses, Enemies, EnemyTransforms);
	LoadSpawnBattlers(AllyClasses, Allies, AllyTransforms);
	InitializeTurn();
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
				FString NewName = SpawnedBattler->GetCharacterName();
				SpawnedBattler->Rename(*NewName);
				BattlerGroup.Add(SpawnedBattler);
			}
		}
	}
}

void ATurnBasedBattle::InitializeTurn() // resets actions to initialize turn
{
	CurrentAction = -1;
	CurrentAllyIndex = 0;
	for (UBattleAction* Action : Actions)
	{
		Action->ResetAction();
	}
	Actions.Empty();
	EvaluateEndBattle();
	GetEnemyActions();
	AllyInputStart();
}

void ATurnBasedBattle::GetEnemyActions()
{
	for (ABattleCharacter * Enemy : Enemies)
	{
		UBattleAction* Action = Enemy->GetRandomAction();
		if (Action)
		{
			Action->SetBattle(this);
			if (Allies.Num() > 0)
			{
				int32 RandomIndex = FMath::RandRange(0, Allies.Num() - 1);
				Action->AddTarget(Allies[RandomIndex]);
			}
			Actions.Add(Action);
		}
		// get speed and convert into ActionOrder.Add(num)
	}
}

void ATurnBasedBattle::AllyInputStart(ABattleCharacter* Ally)
{
	if (!Ally)
	{
		Ally = Allies[0];
	}
	AllyInput = Ally;
	if (TurnBattleWidget)
	{
		TurnBattleWidget->ShowPlayerInput();
	}
	// focus camera on allyinput
}

void ATurnBasedBattle::AllySelectAction(UBattleAction* Action)
{
	if (!Action)
	{
		return;
	}
	AllySelectedAction = Action;
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
	// change camera based on ally possible targets
}

void ATurnBasedBattle::AllySelectTargets(ABattleCharacter* Target)
{
	if (AllyTargets.Contains(Target))
	{
		return;
	}
	AllyTargets.Add(Target);
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
	CurrentAction++;
	CurrentAction = FMath::Clamp(CurrentAction, 0, Actions.Num());
	if (CurrentAction >= Actions.Num())
	{
		InitializeTurn();
		return;
	}
	Actions[CurrentAction]->StartAction();
	//Actions[ActionOrder[CurrentAction]]->StartAction();
}

void ATurnBasedBattle::EvaluateEndBattle()
{
	Super::EvaluateEndBattle();
	for (ABattleCharacter* Enemy : Enemies)
	{
		if (Enemy->GetBattlerParameters()->GetHealth() != 0)
		{
			return;
		}
	}
	EndBattle();
}

void ATurnBasedBattle::EndBattle(FName State, float BlendTime)
{
	Super::EndBattle(State, BlendTime);
}


