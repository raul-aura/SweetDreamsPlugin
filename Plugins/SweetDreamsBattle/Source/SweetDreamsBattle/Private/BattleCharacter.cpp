// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SweetDreamsBPLibrary.h"
#include "BattleNumberWidget.h"
#include "BattleState.h"
#include "BattlerParameterWidget.h"

ABattleCharacter::ABattleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	BattleWorldArea = CreateDefaultSubobject<UBoxComponent>("Battle World Area");
	BattleWorldArea->SetupAttachment(RootComponent);
	ParameterIndicator = CreateDefaultSubobject<UWidgetComponent>("Parameter Indicator");
	ParameterIndicator->SetupAttachment(RootComponent);
	ParameterIndicator->SetDrawSize(FVector2D(1000.f));
	ParameterIndicator->SetTwoSided(true);
	ParameterIndicator->SetPivot(FVector2D(0.5f));

	ParameterIndicatorClass = UBattlerParameterWidget::StaticClass();
	DamageIndicatorClass = UBattleNumberWidget::StaticClass();

	GetCharacterMovement()->MaxWalkSpeed = 900.f;
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateActions();
	ParameterIndicator->SetWidgetClass(ParameterIndicatorClass);
	if (BattlerParams) IndicateParameter(GetBattlerParameters());
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
			State->OnOwnerTick(DeltaTime);
		}
	}
}

FText ABattleCharacter::GetCharacterName() const
{
	return CharacterName;
}

void ABattleCharacter::SetCharacterName(FText NewName)
{
	if (NewName.IsEmpty()) return;
	CharacterName = NewName;
}

UBattlerDataComponent* ABattleCharacter::GetBattlerParameters() const
{
	UBattlerDataComponent* BattlerDataComponent = Cast<UBattlerDataComponent>(GetComponentByClass(UBattlerDataComponent::StaticClass()));
	return BattlerDataComponent;
}

void ABattleCharacter::SetIsInBatte(bool bNewIsBattle)
{
	bIsInBattle = bNewIsBattle;
}

bool ABattleCharacter::GetIsAbleToAct() const
{
	return bIsAbleToAct;
}

void ABattleCharacter::SetIsAbleToAct(bool bNewAble)
{
	bIsAbleToAct = bNewAble;
}

void ABattleCharacter::OnAttack_Implementation()
{
	if (bAttackOnCooldown) return;
	if (bAttackStopsMovement) bCanMove = false;
	bIsAttacking = true;
	bAttackOnCooldown = true;
	FTimerHandle AttackCooldownTimer;
	FTimerHandle LocalTimer;
	GetWorldTimerManager().SetTimer(AttackCooldownTimer, [this] {
		bAttackOnCooldown = false;
		if (bAttackStopsMovement) bCanMove = true;
		}, AttackCooldown, false);
	GetWorldTimerManager().SetTimer(LocalTimer, [this] {
		bIsAttacking = false;
		}, GetWorld()->GetDeltaSeconds(), false);
}

float ABattleCharacter::OnDamageReceived_Implementation(float Damage, bool bIsDamageMitigated)
{
	FTimerHandle AttackTimer;
	bIsAttacked = true;
	GetWorldTimerManager().SetTimer(AttackTimer, [this]() {
		bIsAttacked = false;
		}, GetWorld()->GetDeltaSeconds(), false);
	if (bIsDamageMitigated)
	{
		Damage = MitigateDamage(Damage);
	}
	return FMath::Max(Damage, 0.f);
}

float ABattleCharacter::MitigateDamage_Implementation(float Damage)
{
	return (Damage - GetBattlerParameters()->GetResistence());
}

void ABattleCharacter::IndicateDamage_Implementation(float Value, bool bIsHealInstead)
{
	FVector Origin = BattleWorldArea->GetComponentLocation();
	FVector Extent = BattleWorldArea->GetUnscaledBoxExtent() - IndicatorPadding;
	FVector DamageLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	if (!DamageIndicatorClass) return;
	UWidgetComponent* DamageIndicator = NewObject<UWidgetComponent>(BattleWorldArea, UWidgetComponent::StaticClass());
	if (DamageIndicator)
	{
		DamageIndicator->AttachToComponent(BattleWorldArea, FAttachmentTransformRules::SnapToTargetIncludingScale);
		DamageIndicator->RegisterComponent();
		DamageIndicator->SetWidgetSpace(EWidgetSpace::Screen);
		DamageIndicator->SetDrawSize(FVector2D(1000.f));
		DamageIndicator->SetTwoSided(true);
		DamageIndicator->SetWidgetClass(DamageIndicatorClass);
		DamageIndicator->SetPivot(FVector2D(0.5f));
		DamageIndicator->SetDrawAtDesiredSize(true);
		DamageIndicator->SetWorldLocation(DamageLocation);
		UBattleNumberWidget* DamageWidget = Cast<UBattleNumberWidget>(DamageIndicator->GetWidget());
		if (DamageWidget)
		{
			DamageWidget->OwningWidgetComponent = DamageIndicator;
			DamageWidget->OwningBattler = this;
			bIsHealInstead ? DamageWidget->IndicateHeal(Value) : DamageWidget->IndicateDamage(Value);
		}
	}
}

void ABattleCharacter::RemoveDamageIndicator(UWidgetComponent* Component)
{
	Component->DestroyComponent();
}

void ABattleCharacter::IndicateParameter_Implementation(UBattlerDataComponent* BattlerParameters)
{
	UBattlerParameterWidget* ParameterWidget = Cast<UBattlerParameterWidget>(ParameterIndicator->GetWidget());
	if (ParameterWidget) ParameterWidget->UpdateParameters(BattlerParameters);
}

void ABattleCharacter::OnKilled_Implementation()
{
	ResetActions();
}

UBattleAction* ABattleCharacter::GetRandomAction() const
{
	if (Actions.Num() == 0) return nullptr;
	float TotalWeight = 0.f;
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
	float AccWeight = 0.f;
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

void ABattleCharacter::AddState(TSubclassOf<UBattleState> State, UObject* StateInstigator)
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

