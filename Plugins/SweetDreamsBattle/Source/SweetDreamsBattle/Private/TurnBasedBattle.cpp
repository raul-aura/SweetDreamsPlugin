// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedBattle.h"
#include "BattleInputAction.h"
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
	StartTurn();
}

void ATurnBasedBattle::LoadBattlers_Implementation()
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
				FString IndexLetter = FString::Chr('A' + i);
				FText IndexText = FText::FromString(IndexLetter);
				FText CombinedText = FText::Format(FText::FromString("{0} {1}"), NewName, IndexText);
				if (BattlerGroup == Enemies)
				{
					SpawnedBattler->SetActorLabel(CombinedText.ToString());
					SpawnedBattler->SetCharacterName(CombinedText);
				}
				else
				{
					SpawnedBattler->SetActorLabel(NewName.ToString());
				}
				TArray<UBattleAction*> BattlerAction;
				for (UBattleAction* Action : BattlerAction)
				{
					Action->SetBattle(this);
				}
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

UTurnBasedBattleWidget* ATurnBasedBattle::GetTurnBattleWidget() const
{
	return TurnBattleWidget;
}

void ATurnBasedBattle::GetTargetsAllPossible(UBattleAction*& Action, bool bUpdateCameraView)
{
	TArray<ABattleCharacter*> Targets;
	bool bRemoveDead = true;
	int32 ViewIndex = 0;
	ETargetType TargetType = Action->GetTargetType();
	switch (TargetType)
	{
	case ETargetType::Ally:
		ViewIndex = 1;
		Targets = Allies;
		if (!Action->GetIfIncludeSelf())
		{
			Allies.Remove(Action->GetOwner());
		}
		break;
	case ETargetType::DeadAlly:
		Targets = Allies;
		ViewIndex = 1;
		bRemoveDead = false;
		for (ABattleCharacter* Ally : Targets)
		{
			if (!Ally->GetBattlerParameters()->IsDead())
			{
				Targets.Remove(Ally);
			}
		}
		break;
	case ETargetType::Enemy:
		ViewIndex = 2;
		Targets = Enemies;
		break;
	case ETargetType::Self:
		ViewIndex = 3;
		Targets = Action->GetOwnerAsArray();
		break;
	default:
		ViewIndex = 0;
		break;
	}
	Action->SetTarget(Targets, bRemoveDead);
	if (bUpdateCameraView)
	{
		ECameraView NewView = StaticCast<ECameraView>(ViewIndex);
		ChangeCameraView(NewView, Action->GetOwner(), BattlerBlendTime);
	}
}

void ATurnBasedBattle::StartTurn()
{
	if (EvaluateEndBattle()) return;
	for (ABattleCharacter* Battler : AllBattlers)
	{
		Battler->UpdateActionsCooldown();
		TArray<UBattleState*> States = Battler->GetAllStates();
		if (States.Num() > 0)
		{
			for (UBattleState* State : States)
			{
				State->ConsumeLifetime(EStateLifetime::Turn);
				State->OnTurnStart();
			}
		}
	}
	CurrentAction = 0;
	Actions.Empty();
	LoadTurnActions(Allies, true);
	LoadTurnActions(Enemies, false);
	StartTurnAction();
}

void ATurnBasedBattle::LoadTurnActions(TArray<ABattleCharacter*> Battlers, bool bIsAlly)
{
	for (ABattleCharacter* Battler : Battlers)
	{
		if (Battler->GetBattlerParameters()->IsDead() || Battler->IsPendingKill())
		{
			continue;
		}
		int32 ActionsToAdd = 1 + Battler->GetBattlerParameters()->GetAdditionalActions();
		for (int32 Index = 0; Index < ActionsToAdd; Index++)
		{
			if (bIsAlly)
			{
				UBattleInputAction* InputAction = NewObject<UBattleInputAction>(Battler, UBattleInputAction::StaticClass());
				InputAction->SetWidget(TurnBattleWidget);
				InputAction->SetBattle(this);
				InputAction->SetOwner(Battler);
				AddTurnAction(InputAction, true);
			}
			else
			{
				UBattleAction* EnemyAction = Battler->GetRandomAction();
				if (EnemyAction)
				{
					EnemyAction->SetTargetRandom(Allies, EnemyAction->GetTargetAmount());
					AddTurnAction(EnemyAction);
				}
			}
		}
	}
}

void ATurnBasedBattle::AddTurnAction(UBattleAction* Action, bool bIgnoreSpeed)
{
	if (!Action) return;
	Action->GetOwner()->GetBattlerParameters()->ReceiveManaConsume(Action->GetActionCost());
	if (bIgnoreSpeed)
	{
		Actions.Add(Action);
		return;
	}
	int32 InsertIndex = 0;
	for (; InsertIndex < Actions.Num(); ++InsertIndex)
	{
		if (Action->GetActionSpeed() >= Actions[InsertIndex]->GetActionSpeed())
		{
			break;
		}
	}
	Actions.Insert(Action, InsertIndex);
}

void ATurnBasedBattle::StartTurnAction()
{
	if (Actions.Num() <= 0 || EvaluateEndBattle()) return;
	if (CurrentAction >= Actions.Num())
	{
		StartTurn();
		return;
	}
	UBattleAction* CurrentActionRef = Actions[CurrentAction++];
	CurrentActionBattler = CurrentActionRef->GetOwner();
	ChangeCameraFocus(CurrentActionRef->GetOwner(), BattlerBlendTime);
	FTimerHandle LocalHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(CurrentActionRef, FName("StartAction"), true);
	GetWorldTimerManager().SetTimer(LocalHandle, TimerDel, BattlerBlendTime + ActionDelay, false);
}

void ATurnBasedBattle::EndBattle(FName State, float BlendTime)
{
	CurrentAction = 0;
	for (ABattleCharacter* Battler : AllBattlers)
	{
		Battler->ResetActions();
	}
	Actions.Empty();
	Super::EndBattle(State, BlendTime);
}


