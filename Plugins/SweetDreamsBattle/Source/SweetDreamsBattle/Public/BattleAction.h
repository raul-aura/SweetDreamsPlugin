// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "BattleCharacter.h"
#include "BattleElement.h"
#include "BattleAction.generated.h"

class ASweetDreamsBattleManager;

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	Ally UMETA(DisplayName = "Allies Only"),
	Enemy UMETA(DisplayName = "Enemies Only"),
	Self UMETA(DisplayName = "Self Only"),
};

UCLASS()
class SWEETDREAMSBATTLE_API UBattleAction : public UBattleElement
{
	GENERATED_BODY()
	
public:
	// ACTION CONTROL
	UFUNCTION(BlueprintImplementableEvent)
	void StartAction();
	void UpdateTimer(float Delay);
	virtual void EndAction();

	// OWNERSHIP & TARGET
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action")
	virtual void ResetAction();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action|Target")
	virtual ETargetType GetTargetType() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action|Target")
	virtual int32 GetTargetAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action|Target")
	virtual bool GetIfIncludeSelf() const;

	// BATTLE FUNCTIONS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action")
	virtual void ActionDelay(float DelayTime = 1.0f);

protected:
	FTimerHandle ActionTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (DisplayName = "Targets"))
	ETargetType TargetType = ETargetType::Ally;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (EditCondition = "TargetType==ETargetType::Ally"))
	bool bIncludeSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", meta = (ClampMin = "0"))
	int32 TargetAmount = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta = (ClampMin = "0", ClampMax = "1"))
	float PriorityWeigth = 1; // create weighted priority when Randomizing Action

	//add functions to deal damage, heal, add/remove state, change camera, focus on target, call animation 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Turn-Based Battle")
	bool bTurnBasedAction = false;
};
