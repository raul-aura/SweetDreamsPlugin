// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleState.h"
#include "Components/ActorComponent.h"
#include "BattlerDataComponent.generated.h"


UCLASS(ClassGroup = ("SweetDreams"), meta = (BlueprintSpawnableComponent))
class SWEETDREAMSBATTLE_API UBattlerDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattlerDataComponent();

protected:
	virtual void BeginPlay() override;

	// PARAMS
	UPROPERTY(EditAnywhere, Category = "Params")
	float Health = 100;
	UPROPERTY(EditAnywhere, Category = "Params")
	float Force = 10;
	UPROPERTY(EditAnywhere, Category = "Params")
	float Resistence = 10;
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
	virtual float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetForce() const;
	UFUNCTION(BlueprintCallable)
	virtual float GetResistence() const;
};
