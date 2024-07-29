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

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetOwner(ABattleCharacter* InputOwner);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual ABattleCharacter* GetOwner() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual TArray<ABattleCharacter*> GetOwnerAsArray() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual FText GetElementName() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual FText GetElementDescription() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void UpdateElementDescription(FText NewDescription);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetBattle(ASweetDreamsBattleManager* Battle);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void AddTarget(ABattleCharacter* Target, bool bRemoveDead = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void RemoveTarget(ABattleCharacter* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetTarget(TArray<ABattleCharacter*> NewTargets, bool bRemoveDead = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void SetTargetRandom(TArray<ABattleCharacter*> PossibleTargets, int32 TargetAmount, bool bRemoveDead = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual bool UpdateValidTargets();
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element", meta = (ReturnDisplayName = "All Targets Killed"))
	virtual bool DamageTargets(TArray<ABattleCharacter*> Targets, float& PostMitigatedDamage, int32& KilledTargets, float Damage = 100.0f, bool bCanBeMitigated = true);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void HealTargets(TArray<ABattleCharacter*> Targets, float& HealedAmount, float& OverhealAmount, float Heal = 100.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element", meta = (ReturnDisplayName = "All States Added"))
	virtual bool AddStatesToTargets(TArray<TSubclassOf<UBattleState>> States, TArray<ABattleCharacter*> Targets, int32& StatesAdded, float Chance = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element", meta = (ReturnDisplayName = "All States Removed"))
	virtual bool RemoveStatesOfTargets(TArray<TSubclassOf<UBattleState>> States, TArray<ABattleCharacter*> Targets, int32& StatesRemoved, float Chance = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void CleanseTargets(TArray<ABattleCharacter*> Targets, int32& StatesRemoved);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void KillTargets(TArray<ABattleCharacter*> Targets);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void ReviveTargets(TArray<ABattleCharacter*> Targets);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element", meta = (ReturnDisplayName = "Animation Length"))
	virtual float StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Element")
	virtual void ForceAction(TSubclassOf<UBattleAction> Action, TArray<ABattleCharacter*> Targets, bool bUseCooldown = true);

protected:

	// ELEMENT
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG|Element")
	ABattleCharacter* Owner;
	UPROPERTY(BlueprintReadOnly, Category = "Sweet Dreams RPG|Element")
	ASweetDreamsBattleManager* CurrentBattle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sweet Dreams RPG|Element")
	FText ElementName = FText::FromString(TEXT("Element"));
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sweet Dreams RPG|Element", meta = (MultiLine = true))
	FText ElementDescription = FText::FromString(TEXT("Write about your element here..."));
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Sweet Dreams RPG|Element")
	TArray<ABattleCharacter*> ElementTargets;
	//
	virtual bool AreTargetsValid(TArray<ABattleCharacter*>& Targets);
};
