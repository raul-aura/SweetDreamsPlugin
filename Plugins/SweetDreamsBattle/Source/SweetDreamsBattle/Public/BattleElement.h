// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleElement.generated.h"

class ABattleCharacter;
class UBattleAction;

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetOwner(ABattleCharacter* InputOwner);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual ABattleCharacter* GetOwner() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual FText GetName() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetBattle(ASweetDreamsBattleManager* Battle);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void AddTarget(ABattleCharacter* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetTarget(TArray<ABattleCharacter*> NewTargets);
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element", meta = (ReturnDisplayName = "All Targets Killed"))
	virtual bool DamageTargets(TArray<ABattleCharacter*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage = 100.0f, bool bCanBeMitigated = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void HealTargets(TArray<ABattleCharacter*> Targets, float& HealedAmount, float& OverhealAmount, float Heal = 100.0f);
	// virtual bool AddStateTargets(TArray<TSubclassOf<UBattleState>> States, float Chance = 1.0f, TArray<ABattleCharacter*> Targets, int32& StatesApplied);
	// virtual bool RemoveStateTargets(TArray<TSubclassOf<UBattleState>> States, float Chance = 1.0f, TArray<ABattleCharacter*> Targets, int32& StatesRemoved);
	// virtual bool CleanseTargets(TArray<ABattleCharacter*> Targets, int32& StatesRemoved);
	// virtual void KillTargets(TArray<ABattleCharacter*> Targets);
	// virtual float StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets);
	// virtual void ForceAction(TSubClassOf<UBattleAction>, TArray<ABattleCharacter*> Targets);

protected:

	// ELEMENT
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG|Element")
	ABattleCharacter* Owner;
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG|Element")
	ASweetDreamsBattleManager* CurrentBattle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sweet Dreams RPG|Element")
	FText ElementName = FText::FromString(TEXT("Element"));
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Sweet Dreams RPG|Element")
	TArray<ABattleCharacter*> ElementTargets;
};
