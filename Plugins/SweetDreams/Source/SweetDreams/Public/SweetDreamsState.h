// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsCore.h"
#include "GameFramework/Actor.h"
#include "SweetDreamsState.generated.h"

class USweetDreamsCore;

UCLASS(Abstract, Blueprintable)
class SWEETDREAMS_API ASweetDreamsState : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|State")
	void StartState();
	UFUNCTION(BlueprintNativeEvent, Category = "Sweet Dreams|State")
	void EndState();

	UFUNCTION(BlueprintGetter, Category = "Sweet Dreams|State")
	FName GetStateName() const;

	virtual void StartState_Implementation();
	virtual void EndState_Implementation();

protected:
	// CLASSES
	UWorld* World;
	APlayerController* Player;
	USweetDreamsCore* CoreState;

	UPROPERTY(BlueprintGetter = GetStateName, Category = "Dream State", EditAnywhere)
	FName StateName = "MyState";
};

UCLASS()
class SWEETDREAMS_API ADreamStateAwake : public ASweetDreamsState
{
	GENERATED_BODY()

public:
	ADreamStateAwake();
	virtual void StartState_Implementation() override;
};

UCLASS()
class SWEETDREAMS_API ADreamStateAwakeUI : public ASweetDreamsState
{
	GENERATED_BODY()

public:
	ADreamStateAwakeUI();
	virtual void StartState_Implementation() override;
};

UCLASS()
class SWEETDREAMS_API ADreamStateAsleep : public ASweetDreamsState
{
	GENERATED_BODY()

public:
	ADreamStateAsleep();
	virtual void StartState_Implementation() override;
};
