// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "BattleElement.h"
#include "LevelSequence.h"
#include "BattleAction.generated.h"

class ASweetDreamsBattleManager;

UCLASS()
class SWEETDREAMSBATTLE_API UBattleAction : public UBattleElement
{
	GENERATED_BODY()
	
public:
	// ACTION CONTROL
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void StartAction(bool bUseCooldown);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void StartActionForced(bool bUseCooldown, int32 Turn);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void RefreshCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void UpdateCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual float GetPriorityWeight() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action", meta = (DisplayName = "Remove Self From Battle"))
	virtual void RemoveSelfBattle();
	// OWNERSHIP & TARGET
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void ResetAction();
	virtual void ResetSkipAction();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual bool IsActionAvailable() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action|Target")
	virtual ETargetType GetTargetType() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action|Target")
	virtual int32 GetTargetAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action|Target")
	virtual bool GetIfIncludeSelf() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual int32 GetActionSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void SetActionSpeed(int32 NewSpeed);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual float GetActionCost() const;
	//
	virtual float StartAnimation(UAnimSequence* Animation, TArray<ABattleCharacter*> Targets) override;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void MoveToTarget(ABattleCharacter* Target, int32 MovementID);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void ReturnToPosition(float Delay = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void PlayLevelSequence(ULevelSequence* Sequence);

	UPROPERTY(BlueprintReadWrite, Category = "Action")
	bool bSkipThis = false;
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	int32 TurnToStart = -1;

protected:

	// ACTION CONTROL
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnActionStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnActionEnd();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnMovementComplete(int32 MovementID);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnAnimationComplete(UAnimSequence* Animation);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void OnSequenceComplete(ULevelSequence* Sequence);
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void EndAction(float Delay = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void StartNextTurnAction(float Delay = 1.0f);
	//
	FTimerHandle ActionTimer;
	FTimerHandle ActionCooldown;
	UPROPERTY(BlueprintReadWrite)
	int32 TurnsPassed = 0;
	UPROPERTY(BlueprintReadWrite)
	bool bIsOnCooldown = false;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (DisplayName = "Targets"))
	ETargetType TargetType = ETargetType::Ally;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (EditCondition = "TargetType==ETargetType::Ally"))
	bool bIncludeSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (ClampMin = "1"))
	int32 TargetAmount = 1;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ClampMin = "0"))
	float Cost = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (DisplayName = "Cooldown (Seconds)", ClampMin = "0", EditCondition = "bTurnBasedAction==false", EditConditionHides))
	float Cooldown = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (DisplayName = "Cooldown (Turns)", ClampMin = "0", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	int32 CooldownTurns = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ClampMin = "0.1", ClampMax = "1"))
	float PriorityWeigth = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ClampMin = "0", ClampMax = "9999", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	int32 ActionSpeed = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bTurnBasedAction = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings", meta = (EditCondition = "bTurnBasedAction==true", EditConditionHides))
	bool bOverrideOwnerSpeed = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings", meta = (DisplayName = "Is Last Action when Forced", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	bool bAddedLast = false;
};
