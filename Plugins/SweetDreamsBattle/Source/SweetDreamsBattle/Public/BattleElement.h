// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleElement.generated.h"

class ABattleCharacter;
class UBattleAction;

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Ally UMETA(DisplayName = "Allies"),
	DeadAlly UMETA(DisplayName = "Dead Allies"),
	Enemy UMETA(DisplayName = "Enemies Only"),
	Self UMETA(DisplayName = "Self Only"),
};

UCLASS(Blueprintable, BlueprintType)
class SWEETDREAMSBATTLE_API UBattleElement : public UObject
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetOwner(ABattleCharacter* InputOwner);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual ABattleCharacter* GetOwner() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual TArray<ABattleCharacter*> GetOwnerAsArray() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	FText GetElementName() const;
	virtual FText GetElementName_Implementation() const { return ElementName; }
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	FText GetElementDescription() const;
	virtual FText GetElementDescription_Implementation() const { return ElementDescription; }
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void UpdateElementDescription(FText NewDescription);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetBattle(ASweetDreamsBattleManager* Battle);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void AddTarget(ABattleCharacter* Target, bool bRemoveDead = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void RemoveTarget(ABattleCharacter* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetTarget(TArray<ABattleCharacter*> NewTargets, bool bRemoveDead = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void SetTargetRandom(TArray<ABattleCharacter*> PossibleTargets, int32 TargetAmount, bool bRemoveDead = true);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Element")
	virtual TArray<ABattleCharacter*> GetAdjacentTargets(ABattleCharacter* PrimaryTarget, TArray<ABattleCharacter*> TargetsToSearch);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual bool UpdateValidTargets();
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "All Targets Killed"))
	bool DamageTargets(TArray<ABattleCharacter*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage = 100.0f, bool bCanBeMitigated = true, bool bApplyCalculations = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	void HealTargets(TArray<ABattleCharacter*> Targets, float& HealedAmount, float& OverhealAmount, float Heal = 100.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "All States Added"))
	static bool AddStatesToTargets(UObject* StateInstigator, TArray<TSubclassOf<UBattleState>> States, TArray<ABattleCharacter*> Targets, int32& StatesAdded, float Chance = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "All States Removed"))
	static bool RemoveStatesOfTargets(TArray<TSubclassOf<UBattleState>> States, TArray<ABattleCharacter*> Targets, int32& StatesRemoved, float Chance = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	static void CleanseTargets(TArray<ABattleCharacter*> Targets, int32& StatesRemoved);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	static void KillTargets(TArray<ABattleCharacter*> Targets);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	static void ReviveTargets(TArray<ABattleCharacter*> Targets, float HealthRestore = 100.f, float ManaRestore = 100.f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element", meta = (ReturnDisplayName = "Animation Length"))
	virtual float StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Element")
	virtual void ForceAction(TSubclassOf<UBattleAction> Action, TArray<ABattleCharacter*> Targets, bool bUseCooldown = true, int32 Turn = -1);

protected:

	// ELEMENT
	UPROPERTY(BlueprintReadOnly, Category = "Battle Element")
	ABattleCharacter* Owner;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Element")
	ASweetDreamsBattleManager* CurrentBattle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Element")
	FText ElementName = FText::FromString(TEXT("Element"));
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Element", meta = (MultiLine = true))
	FText ElementDescription = FText::FromString(TEXT("Write about your element here..."));
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Battle Element")
	TArray<ABattleCharacter*> ElementTargets;
	//
	UFUNCTION(BlueprintCallable, Category = "Battle Element", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool AreTargetsValid(const TArray<ABattleCharacter*>& Targets);
};
