// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleElement.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetOwner(ABattleCharacter* InputOwner);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetBattle(ASweetDreamsBattleManager* Battle);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void AddTarget(ABattleCharacter* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetTarget(TArray<ABattleCharacter*> NewTargets);

	// virtual void DamageTargets();
	// virtual void HealTargets();
	// virtual void AddStateTargets();
	// virtual void RemoveStateTargets();
	// virtual void CleanseTargets();
	// virtual void KillTargets();
protected:

	// ELEMENT
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG|Element")
	ABattleCharacter* Owner;
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG|Element")
	ASweetDreamsBattleManager* CurrentBattle;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Sweet Dreams RPG|Element")
	TArray<ABattleCharacter*> Targets;
};
