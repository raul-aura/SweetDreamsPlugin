// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBasedBattle.h"
#include "BattleInputAction.h"
#include "BattleState.h"
#include "Kismet/GameplayStatics.h"

ATurnBasedBattle::ATurnBasedBattle()
{
	AllyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Allies"));
	AllyRoot->SetupAttachment(BattleRoot);

	EnemyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Enemies"));
	EnemyRoot->SetupAttachment(BattleRoot);
}

void ATurnBasedBattle::BeginPlay()
{
	Super::BeginPlay();
	if (BattleWidget)
	{
		TurnBattleWidget = Cast<UTurnBasedBattleWidget>(BattleWidget);
	}
}

void ATurnBasedBattle::StartBattle(float BlendTime)
{
	Super::StartBattle(BlendTime);
	TurnBattleWidget->SetBattleManager(this);
	GetWorldTimerManager().SetTimer(BattleTimer, this, &ATurnBasedBattle::StartTurn, FirstTurnDelay + BlendTime, false);
	if (TurnBattleWidget) TurnBattleWidget->OnBattleStarted();
	if (Player)
	{
		FInputModeGameAndUI NewMode;
		if (TurnBattleWidget) NewMode.SetWidgetToFocus(TurnBattleWidget->TakeWidget());
		Player->SetInputMode(NewMode);
		Player->SetShowMouseCursor(true);
	}
}

void ATurnBasedBattle::EvaluateTransforms()
{
	AllyTransforms.Empty();
	TArray<USceneComponent*> AllyComponents;
	AllyRoot->GetChildrenComponents(false, AllyComponents);
	FVector AllyRootLocation = AllyRoot->GetComponentLocation();
	AllyComponents.Sort([AllyRootLocation](const USceneComponent& A, const USceneComponent& B)
		{
			return FVector::DistSquared(A.GetComponentLocation(), AllyRootLocation) < FVector::DistSquared(B.GetComponentLocation(), AllyRootLocation);
		});
	for (USceneComponent* Component : AllyComponents)
	{
		FTransform LocalTransform = Component->GetRelativeTransform();
		AllyTransforms.Add(LocalTransform);
	}
	EnemyTransforms.Empty();
	TArray<USceneComponent*> EnemyComponents;
	EnemyRoot->GetChildrenComponents(false, EnemyComponents);
	FVector EnemyRootLocation = EnemyRoot->GetComponentLocation();
	EnemyComponents.Sort([EnemyRootLocation](const USceneComponent& A, const USceneComponent& B)
		{
			return FVector::DistSquared(A.GetComponentLocation(), EnemyRootLocation) < FVector::DistSquared(B.GetComponentLocation(), EnemyRootLocation);
		});
	for (USceneComponent* Component : EnemyComponents)
	{
		FTransform LocalTransform = Component->GetRelativeTransform();
		EnemyTransforms.Add(LocalTransform);
	}
	TArray<USceneComponent*> AllComponents;
	AllComponents.Append(AllyComponents);
	AllComponents.Append(EnemyComponents);
	for (USceneComponent* Component : AllComponents)
	{
		Component->SetHiddenInGame(true, false);
	}
}

void ATurnBasedBattle::LoadBattlers_Implementation()
{
	LoadSpawnBattlers(EnemyClasses, Enemies, EnemyRoot, EnemyTransforms);
	LoadSpawnBattlers(AllyClasses, Allies, AllyRoot, AllyTransforms);
	Super::LoadBattlers_Implementation();
}

bool ATurnBasedBattle::EvaluateEndBattle_Implementation()
{
	if (MaxTurns > 0 && CurrentTurn >= MaxTurns)
	{
		bIsVictorious = bVictoryWhenMaxTurns;
		EndBattle();
		return true;
	}
	return Super::EvaluateEndBattle_Implementation();
}

void ATurnBasedBattle::LoadSpawnBattlers(TArray<TSoftClassPtr<ABattleCharacter>> Battlers, TArray<ABattleCharacter*>& BattlerGroup, USceneComponent* BattlerRoot, TArray<FTransform> TransformGroup)
{
	int32 Length = FMath::Min(Battlers.Num(), TransformGroup.Num());
	if (Length <= 0) return;
	for (int32 Index = 0; Index < Length; Index++)
	{
		TSubclassOf<ABattleCharacter> LoadedBattler = Battlers[Index].LoadSynchronous();
		if (LoadedBattler)
		{
			ABattleCharacter* SpawnedBattler = GetWorld()->SpawnActor<ABattleCharacter>(LoadedBattler);
			if (SpawnedBattler)
			{
				SpawnedBattler->AttachToComponent(BattlerRoot, FAttachmentTransformRules::KeepWorldTransform);
				if (TransformGroup.IsValidIndex(Index))
				{
					SpawnedBattler->SetActorRelativeTransform(TransformGroup[Index]);
				}
				SpawnedBattler->GetDreamController()->SetOrigin(SpawnedBattler);
				FText NewName = SpawnedBattler->GetCharacterName();
				SpawnedBattler->SetIsInBatte(true);
				if (BattlerGroup == Enemies)
				{
					if (IsNameEqual(BattlerGroup, NewName))
					{
						FString IndexLetter = FString::Chr('A' + Index);
						FText IndexText = FText::FromString(IndexLetter);
						FText CombinedText = FText::Format(FText::FromString("{0} {1}"), NewName, IndexText);
						SpawnedBattler->SetCharacterName(CombinedText);
					}
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

bool ATurnBasedBattle::IsNameEqual(const TArray<ABattleCharacter*>& BattlerGroup, const FText& NameToCheck)
{
	FString OriginalName = NameToCheck.ToString();
	if (OriginalName.Len() > 2 && FChar::IsAlpha(OriginalName.Right(1)[0]) && OriginalName.Right(2).Left(1) == " ")
	{
		OriginalName = OriginalName.LeftChop(2);
	}
	FText StrippedNameToCheck = FText::FromString(OriginalName);
	for (ABattleCharacter* Battler : BattlerGroup)
	{
		FString ExistingNameString = Battler->GetCharacterName().ToString();
		if (ExistingNameString.Len() > 2 && FChar::IsAlpha(ExistingNameString.Right(1)[0]) && ExistingNameString.Right(2).Left(1) == " ")
		{
			ExistingNameString = ExistingNameString.LeftChop(2);
		}
		FText StrippedExistingName = FText::FromString(ExistingNameString);
		if (StrippedExistingName.EqualTo(StrippedNameToCheck))
		{
			return true;
		}
	}
	return false;
}

void ATurnBasedBattle::GetAlliesFromArray(TArray<TSoftClassPtr<ABattleCharacter>> NewAllies)
{
	if (NewAllies.Num() <= 0) return;
	AllyClasses = NewAllies;
}

void ATurnBasedBattle::GetEnemiesFromArray(TArray<TSoftClassPtr<ABattleCharacter>> NewEnemies)
{
	if (NewEnemies.Num() <= 0) return;
	EnemyClasses = NewEnemies;
}

void ATurnBasedBattle::LoadEnemiesGroup(int32 Index)
{
	if (EnemyGroups.IsValidIndex(Index))
	{
		FEnemyGroups Group = EnemyGroups[Index];
		if (Group.EnemyGroup.Num() > 0)
		{
			GetEnemiesFromArray(Group.EnemyGroup);
		}
	}
}

TArray<FTransform> ATurnBasedBattle::GetRandomAllyTransforms() const
{
	return GetRandomTransforms(Allies, AllyTransforms);
}

TArray<FTransform> ATurnBasedBattle::GetRandomEnemyTransforms() const
{
	return GetRandomTransforms(Enemies, EnemyTransforms);
}

TArray<FTransform> ATurnBasedBattle::GetRandomTransforms(const TArray<ABattleCharacter*>& BattlerGroup, const TArray<FTransform>& TransformGroup) const
{
	float Tolerance = 10.0f;
	TArray<FTransform> LocalTransform;
	if (BattlerGroup.Num() <= 0) return LocalTransform;
	for (const FTransform& Transform : TransformGroup)
	{
		bool bIsTransformUsed = false;
		for (ABattleCharacter* Battler : BattlerGroup)
		{
			if (Battler && !Battler->GetBattlerParameters()->IsDead())
			{
				FVector Location = Battler->GetActorLocation();
				FVector TransformLocation = Transform.GetLocation();

				if (FVector::Dist(Location, TransformLocation) <= Tolerance)
				{
					bIsTransformUsed = true;
					break;
				}
			}
		}
		if (!bIsTransformUsed)
		{
			LocalTransform.Add(Transform);
		}
	}
	return LocalTransform;
}

UTurnBasedBattleWidget* ATurnBasedBattle::GetTurnBattleWidget() const
{
	return TurnBattleWidget;
}

void ATurnBasedBattle::GetTargetsAllPossible(UBattleAction*& Action, bool bUpdateCameraView)
{
	TArray<ABattleCharacter*> Targets;
	bool bRemoveDead = true;
	ETargetType TargetType = Action->GetTargetType();
	ECameraView NewView;
	switch (TargetType)
	{
	case ETargetType::Ally:
		NewView = ECameraView::Allies;
		Targets = Allies;
		if (!Action->GetIfIncludeSelf())
		{
			Targets.Remove(Action->GetOwner());
		}
		break;
	case ETargetType::DeadAlly:
		Targets = Allies;
		NewView = ECameraView::Allies;
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
		NewView = ECameraView::Enemies;
		Targets = Enemies;
		break;
	case ETargetType::Self:
		NewView = ECameraView::Self;
		Targets = Action->GetOwnerAsArray();
		break;
	default:
		NewView = ECameraView::AllBattlers;
		break;
	}
	Action->SetTarget(Targets, bRemoveDead);
	if (bUpdateCameraView && bAutoMoveCamera)
	{
		ChangeCameraView(NewView, Action->GetOwner(), BattlerBlendTime);
	}
}

ATurnBasedBattle* ATurnBasedBattle::FindActiveTurnBattle(const UObject* WorldContext, int32& BattleId)
{
	return Cast<ATurnBasedBattle>(ASweetDreamsBattleManager::FindActiveBattle(WorldContext, BattleId));
}

void ATurnBasedBattle::StartTurn()
{
	if (EvaluateEndBattle()) return;
	CurrentTurn++;
	for (ABattleCharacter* Battler : AllBattlers)
	{
		if (Battler)
		{
			Battler->UpdateActionsCooldown();
			TArray<UBattleState*> States = Battler->GetAllStates();
			if (States.Num() > 0)
			{
				for (UBattleState* State : States)
				{
					State->ConsumeLifetime(EStateLifetime::Turn);
					State->OnTurnStart(CurrentTurn);
				}
			}
		}
	}
	for (UBattleAction* Action : Actions)
	{
		Action->ResetSkipAction();
	}
	CurrentAction = 0;
	Actions.Empty();
	LoadTurnActions(Allies, true);
	LoadTurnActions(Enemies, false);
	StartTurnAction();
	OnTurnStarted(CurrentTurn);
}

void ATurnBasedBattle::LoadTurnActions(TArray<ABattleCharacter*> Battlers, bool bIsAlly)
{
	for (ABattleCharacter* Battler : Battlers)
	{
		if (!Battler || Battler->GetBattlerParameters()->IsDead() || Battler->IsPendingKill())
		{
			continue;
		}
		TArray<UBattleAction*> AllActions = Battler->GetAllActions();
		for (UBattleAction* Action : AllActions)
		{
			if (Action && Action->TurnToStart == CurrentTurn)
			{
				AddTurnAction(Action);
			}
		}
		int32 ActionsToAdd = 1 + Battler->GetBattlerParameters()->GetAdditionalActions();
		for (int32 Index = 0; Index < ActionsToAdd; Index++)
		{
			if (bIsAlly)
			{
				UBattleInputAction* InputAction = NewObject<UBattleInputAction>(Battler, InputActionClass);
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
	Action->SetBattle(this);
	if (bIgnoreSpeed)
	{
		Actions.Add(Action);
		OnActionAdded(Action);
		return;
	}
	int32 InsertIndex = 0;
	for (; InsertIndex < Actions.Num(); ++InsertIndex)
	{
		if (Actions[InsertIndex]->bSkipThis) continue;
		if (Action->GetActionSpeed() > Actions[InsertIndex]->GetActionSpeed())
		{
			break;
		}
	}
	Actions.Insert(Action, InsertIndex);
	OnActionAdded(Action);
}

bool ATurnBasedBattle::RemoveTurnAction(UBattleAction* Action)
{
	if (!Action) return false;
	if (Actions.Find(Action) != INDEX_NONE)
	{
		OnActionRemoved(Action);
		return true;
	}
	return false;
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
	if (bAutoMoveCamera) ChangeCameraFocus(CurrentActionRef->GetOwner(), BattlerBlendTime);
	FTimerHandle LocalHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(CurrentActionRef, FName("StartAction"), true);
	GetWorldTimerManager().SetTimer(LocalHandle, TimerDel, BattlerBlendTime + ActionDelay, false);
}

void ATurnBasedBattle::EndBattle(float BlendTime)
{
	CurrentAction = 0;
	CurrentTurn = -1;
	if (TurnBattleWidget) TurnBattleWidget->OnBattleEnded(bIsVictorious);
	for (ABattleCharacter* Battler : AllBattlers)
	{
		Battler->ResetActions();
		Battler->SetIsInBatte(false);
	}
	Actions.Empty();
	Super::EndBattle(BlendTime);
}


