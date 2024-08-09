// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleState.h"
#include "Components/ActorComponent.h"
#include "BattlerDataComponent.generated.h"

class ABattleCharacter;

UCLASS(ClassGroup = ("SweetDreams"), Blueprintable, meta = (BlueprintSpawnableComponent))
class SWEETDREAMSBATTLE_API UBattlerDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattlerDataComponent();

protected:
	virtual void BeginPlay() override;

	// PARAMS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params")
	int32 Level;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (DisplayName = "Base Health", ClampMin = "1"))
	float Health = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (DisplayName = "Base Mana", ClampMin = "1"))
	float Mana = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "1"))
	float Force = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "1"))
	float Resistence = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "1"))
	int32 Speed = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (ClampMin = "0", ClampMax = "99"))
	int32 AdditionalActions = 0;
	//
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float CurrentHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float CurrentMana;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	bool bIsDead = false;
	//
	TArray<UBattleState*> States;
	// MULTIPLIERS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "1"))
	float HealthMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "1"))
	float ForceMultiplier = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Multipliers", meta = (Units = "%", ClampMin = "1"))
	float ResistenceMultiplier = 100;
	// MODIFIERS
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Modifiers")
	TArray<float> HealthModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Modifiers")
	TArray<float> ForceModifiers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Modifiers")
	TArray<float> ResistenceModifiers;

	// FUNCTIONS
	UFUNCTION(BlueprintCallable)
	virtual float GetModifiedParameter(TArray<float> Modifiers, float Parameter = 100.0f, float BaseMultiplier = 100.0f) const;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	virtual ABattleCharacter* GetBattlerOwner() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetForce() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetResistence() const;
	UFUNCTION(BlueprintCallable)
	virtual int32 GetSpeed() const;
	UFUNCTION(BlueprintCallable)
	virtual int32 GetAdditionalActions() const;
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName = "Added to Index"))
	virtual int32 AddModifier(UPARAM(ref) TArray<float>& Modifiers, float ModifierToAdd = 10.0f);
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName = "Found and Updated"))
	virtual bool UpdateModifier(UPARAM(ref) TArray<float>& Modifiers, float ModifierToSearch = 10.0f, float NewModifier = 20.0f);
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName = "Found and Updated"))
	virtual bool UpdateModifierAt(UPARAM(ref) TArray<float>& Modifiers, int32 Index = 0, float NewModifier = 20.0f);
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName = "Found and Removed"))
	virtual bool RemoveModifier(UPARAM(ref) TArray<float>& Modifiers, float ModifierToRemove = 10.0f);
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName = "Found and Removed"))
	virtual bool RemoveModifierAt(UPARAM(ref) TArray<float>& Modifiers, int32 Index = 0);
	//
	UFUNCTION(BlueprintCallable)
	virtual float ReceiveDamage(float Damage, bool bCanBeMitigated);
	UFUNCTION(BlueprintCallable)
	virtual float ReceiveHeal(float Heal);
	UFUNCTION(BlueprintCallable)
	virtual float ReceiveManaConsume(float Consume);
	UFUNCTION(BlueprintCallable)
	virtual float ReceiveManaRestore(float Restore);
	UFUNCTION(BlueprintCallable)
	void Kill();
	UFUNCTION(BlueprintCallable)
	void Revive(); 
	UFUNCTION(BlueprintCallable)
	virtual bool IsDead() const;
};
