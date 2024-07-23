// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleState.h"
#include "Components/ActorComponent.h"
#include "BattlerDataComponent.generated.h"

class ABattleCharacter;

UCLASS(ClassGroup = ("SweetDreams"), meta = (BlueprintSpawnableComponent))
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params", meta = (DisplayName = "Base Health"))
	float Health = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params")
	float Force = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params")
	float Resistence = 10;
	//
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	float CurrentHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Params")
	bool bIsDead = false;
	//
	TArray<UBattleState*> States;
	// MULTIPLIERS
	UPROPERTY(EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float HealthMultiplier = 100;
	UPROPERTY(EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ForceMultiplier = 100;
	UPROPERTY(EditAnywhere, Category = "Multipliers", meta = (Units = "%"))
	float ResistenceMultiplier = 100;
	// MODIFIERS
	UPROPERTY(EditAnywhere, Category = "Modifiers")
	TArray<float> HealthModifiers;
	UPROPERTY(EditAnywhere, Category = "Modifiers")
	TArray<float> ForceModifiers;
	UPROPERTY(EditAnywhere, Category = "Modifiers")
	TArray<float> ResistenceModifiers;

	// FUNCTIONS
	UFUNCTION(BlueprintCallable)
	virtual float GetModifiers(TArray<float> Modifiers, float BaseMultiplier = 100) const;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	virtual ABattleCharacter* GetBattlerOwner() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetForce() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetResistence() const;
	//
	UFUNCTION(BlueprintCallable)
	virtual float ReceiveDamage(float Damage, bool bCanBeMitigated);
	UFUNCTION(BlueprintCallable)
	virtual float ReceiveHeal(float Heal);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetMitigatedDamage(float& Mitigated);
	virtual void GetMitigatedDamage_Implementation(float& Mitigated);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Kill();
	virtual void Kill_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Revive();
	virtual void Revive_Implementation();
	UFUNCTION(BlueprintCallable)
	virtual bool IsDead() const;
};
