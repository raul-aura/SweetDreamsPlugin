// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCharacter.h"

ABattleCharacter::ABattleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	Camera->bUsePawnControlRotation = false; 

	BattlerParams = CreateDefaultSubobject<UBattlerDataComponent>(TEXT("Battler Parameters"));
	AddOwnedComponent(BattlerParams);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);
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

}

void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FString ABattleCharacter::GetCharacterName() const
{
	return DisplayName;
}

UBattlerDataComponent* ABattleCharacter::GetBattlerParameters() const
{
	return BattlerParams;
}

UBattleAction* ABattleCharacter::GetRandomAction() const
{
	if (Actions.Num() == 0)
	{
		return nullptr;
	}
	int32 Random = FMath::FRandRange(0, Actions.Num() - 1);
	return Actions[Random];
}

TArray<UBattleAction*> ABattleCharacter::GetAllActions() const
{
	return Actions;
}

