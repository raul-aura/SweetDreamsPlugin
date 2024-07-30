// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "BattleElement.h"
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
	virtual void StartActionForced(bool bUseCooldown);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void RefreshCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void UpdateCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual float GetPriorityWeight() const;
	// OWNERSHIP & TARGET
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void ResetAction();
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
protected:
	// ACTION CONTROL
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionStart();
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionEnd();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	virtual void EndAction(float Delay = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Action")
	void UpdateTimer(float Delay);
	//
	FTimerHandle ActionTimer;
	FTimerHandle ActionCooldown;
	int32 TurnsPassed = 0;
	bool bIsOnCooldown = false;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Action", meta = (DisplayName = "Targets"))
	ETargetType TargetType = ETargetType::Ally;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Action", meta = (EditCondition = "TargetType==ETargetType::Ally"))
	bool bIncludeSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Action", meta = (ClampMin = "1"))
	int32 TargetAmount = 1;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Data", meta = (ClampMin = "0"))
	float Cost = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Data", meta = (DisplayName = "Cooldown (Seconds)", ClampMin = "0", EditCondition = "bTurnBasedAction==false", EditConditionHides))
	float Cooldown = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Data", meta = (DisplayName = "Cooldown (Turns)", ClampMin = "0", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	int32 CooldownTurns = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Data", meta = (ClampMin = "0.1", ClampMax = "1"))
	float PriorityWeigth = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Data", meta = (ClampMin = "0", ClampMax = "9999", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	int32 ActionSpeed = 0;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sweet Dreams|RPG|Settings")
	bool bTurnBasedAction = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sweet Dreams|RPG|Settings", meta = (EditCondition = "bTurnBasedAction==true", EditConditionHides))
	bool bOverrideOwnerSpeed = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sweet Dreams|RPG|Settings", meta = (DisplayName = "Is Last Action when Forced", EditCondition = "bTurnBasedAction==true", EditConditionHides))
	bool bAddedLast = false;
};
