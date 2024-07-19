// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
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
	void OnActionStart();

	// OWNERSHIP & TARGET
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action")
	virtual void ResetAction();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action|Target")
	virtual ETargetType GetTargetType() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action|Target")
	virtual int32 GetTargetAmount() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action|Target")
	virtual bool GetIfIncludeSelf() const;

protected:
	// ACTION CONTROL
	UFUNCTION(BlueprintImplementableEvent)
	void OnActionEnd();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Action")
	virtual void EndAction(float Delay = 1.0f);
	void UpdateTimer(float Delay);
	//
	FTimerHandle ActionTimer;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams RPG|Action", meta = (DisplayName = "Targets"))
	ETargetType TargetType = ETargetType::Ally;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams RPG|Action", meta = (EditCondition = "TargetType==ETargetType::Ally"))
	bool bIncludeSelf = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams RPG|Action", meta = (ClampMin = "0"))
	int32 TargetAmount = 1;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams RPG|Data", meta = (ClampMin = "0"))
	float Cost = 100; // create function to get cost and see if have enough to use
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams RPG|Data", meta = (ClampMin = "0", ClampMax = "1"))
	float PriorityWeigth = 1; // create weighted priority when Randomizing Action
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Sweet Dreams RPG|Settings")
	bool bTurnBasedAction = false;
	//add functions to deal damage, heal, add/remove state, change camera, focus on target, call animation 
	
};
