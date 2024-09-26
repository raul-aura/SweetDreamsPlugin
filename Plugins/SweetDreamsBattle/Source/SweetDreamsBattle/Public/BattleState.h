// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleElement.h"
#include "BattleState.generated.h"

UENUM(BlueprintType)
enum class EStateLifetime : uint8
{
	Permanent UMETA(DisplayName = "Permanent (Until Cleansed)"),
	Turn UMETA(DisplayName = "On Turn Start"),
	Action UMETA(DisplayName = "On Action End"),
	Second UMETA(DisplayName = "Seconds"),
};

UCLASS()
class SWEETDREAMSBATTLE_API UBattleState : public UBattleElement
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UObject* StateInstigator = nullptr;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual UObject* GetStateInstigator() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual TArray<UObject*> GetInstigatorAsArray() const;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<UBattleAction>> TriggerableActions;
	// add an TArray of triggerable actions
	// create enum with State Type: Positive, Negative, None

	// STACKS
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsStackable = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bConsumeStacks = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Auto Consume Stacks on Lifetime"))
	bool bAutoConsumeStacks = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "1"))
	int32 InitialStacks = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "1"))
	int32 MaxStacks = 1;
	UPROPERTY(BlueprintReadOnly)
	int32 Stacks = 0;

	// LIFETIME
	UPROPERTY(BlueprintReadOnly)
	bool bAlreadyAppliedOnce = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EStateLifetime Lifetime = EStateLifetime::Permanent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "Lifetime==EStateLifetime::Turn", ClampMin = "1"))
	int32 TurnsToEnd = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "Lifetime==EStateLifetime::Action", ClampMin = "1"))
	int32 ActionsToEnd = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "Lifetime==EStateLifetime::Second", ClampMin = "0.1"))
	float SecondsToEnd = 0.1f;
	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "Lifetime (Turns/Actions)"))
	int32 IntLifetime = 0;
	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "Lifetime (Seconds)"))
	float FloatLifetime = 0.f;

public:
	// EVENTS
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnApplied();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnTurnStart(int32 Turn = 0);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnActionEnd(UBattleAction* Action);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnOwnerTick(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	void OnRemoved();
	// CREATE EVENTS:
	// OnManaRestored
	// OnHealed
	// OnDamageDealt
	// OnDamageReceived
	
	//
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
	virtual bool IsTriggerableAction(UBattleAction* Action) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ApplyState(UObject* Instigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ResetLifetime();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void AddStacks();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ConsumeStacks();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void ConsumeLifetime(EStateLifetime LifetimeToConsume = EStateLifetime::Action);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual void RemoveState();
	// GETTERS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual int32 GetRemainingLifetime() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	virtual float GetRemainingLifetimeSeconds() const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	FText GetDurationAsText() const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|State")
	FText GetInstigatorAsText() const;
};
