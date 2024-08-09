// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacter.h"

ABattleCharacter::ABattleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Camera->SetRelativeRotation(FRotator(0.0f, 5.0f, -5.0f));

	BattlerParams = CreateDefaultSubobject<UBattlerDataComponent>(TEXT("Battler Parameters"));
	AddOwnedComponent(BattlerParams);
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateActions();
}

void ABattleCharacter::CreateActions()
{
	if (ActionClasses.Num() > 0)
	{
		for (TSoftClassPtr<UBattleAction> ActionClass : ActionClasses)
		{
			TSubclassOf<UBattleAction> ActionClassLoaded = ActionClass.LoadSynchronous();
			UBattleAction* Action = NewObject<UBattleAction>(this, ActionClassLoaded);
			if (Action)
			{
				Action->SetOwner(this);
				Actions.Add(Action);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, "Error creating object Action.");
			}
		}
	}
}

void ABattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (States.Num() > 0)
	{
		for (UBattleState* State : States)
		{
			State->ConsumeLifetime(EStateLifetime::Second);
			State->OnTick();
		}
	}
}

FText ABattleCharacter::GetCharacterName() const
{
	return CharacterName;
}

void ABattleCharacter::SetCharacterName(FText NewName)
{
	if (NewName.EqualTo(FText::FromString(TEXT(""))))
	{
		return;
	}
	CharacterName = NewName;
}

UBattlerDataComponent* ABattleCharacter::GetBattlerParameters() const
{
	return BattlerParams;
}

float ABattleCharacter::OnDamageReceived_Implementation(float Damage, bool bIsDamageMitigated)
{
	if (bIsDamageMitigated)
	{
		Damage -= GetBattlerParameters()->GetResistence();
	}
	return Damage;
}

void ABattleCharacter::OnKilled_Implementation()
{
	ResetActions();
}

UBattleAction* ABattleCharacter::GetRandomAction() const
{
	if (Actions.Num() == 0) return nullptr;
	float TotalWeight = 0.0f;
	TArray<float> Weights;
	for (UBattleAction* Action : Actions)
	{
		float Weight = Action->GetPriorityWeight();
		TotalWeight += Weight;
		Weights.Add(Weight);
	}
	for (float& Weight : Weights)
	{
		Weight /= TotalWeight;
	}
	int32 RandomNum = FMath::FRand();
	float AccWeight = 0.0f;
	for (int32 Index = 0; Index < Weights.Num(); Index++)
	{
		AccWeight += Weights[Index];
		if (AccWeight >= RandomNum)
		{
			return Actions[Index];
		}
	}
	return Actions[0];
}

TArray<UBattleAction*> ABattleCharacter::GetAllActions() const
{
	return Actions;
}

void ABattleCharacter::UpdateActionsCooldown()
{
	if (Actions.Num() == 0) return;
	for (UBattleAction* Action : Actions)
	{
		Action->UpdateCooldown();
	}
}

void ABattleCharacter::ResetActions()
{
	if (Actions.Num() == 0) return;
	for (UBattleAction* Action : Actions)
	{
		Action->ResetAction();
	}
}

void ABattleCharacter::AddState(TSubclassOf<UBattleState> State, ABattleCharacter* StateInstigator)
{
	if (!State || !StateInstigator) return;
	UBattleState* NewState = nullptr;
	for (UBattleState* ForState : States)
	{
		if (ForState->IsA(State))
		{
			NewState = ForState;
			break;
		}
	}
	if (!NewState)
	{
		NewState = NewObject<UBattleState>(this, State);
		NewState->SetOwner(this);
		States.Add(NewState);
	}
	NewState->ApplyState(StateInstigator);
}

void ABattleCharacter::RemoveState(TSubclassOf<UBattleState> State)
{
	if (!State) return;
	for (UBattleState* LocalState : States)
	{
		if (LocalState->IsA(State))
		{
			LocalState->OnRemoved();
			States.Remove(LocalState);
			LocalState->ConditionalBeginDestroy();
			return;
		}
	}
}

int32 ABattleCharacter::RemoveAllStates()
{
	int32 StatesRemoved = 0;
	for (UBattleState* State : States)
	{
		State->OnRemoved();
		States.Remove(State);
		State->ConditionalBeginDestroy();
		StatesRemoved++;
	}
	return StatesRemoved;
}

TArray<UBattleState*> ABattleCharacter::GetAllStates() const
{
	return States;
}

