// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedBattleWidget.h"
#include "SweetDreamsBattleManager.h"
#include "TurnBasedBattle.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API ATurnBasedBattle : public ASweetDreamsBattleManager
{
	GENERATED_BODY()
	
public:
	ATurnBasedBattle();

protected:
	virtual void BeginPlay() override;

	// UI
	UTurnBasedBattleWidget* TurnBattleWidget;

	// BATTLERS
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Battlers")
	TArray<ABattleCharacter*> AllBattlers;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* PlayerRoot; 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* EnemyRoot;
	//
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Battlers", meta = (DisplayName = "Enemies"))
	TArray<TSoftClassPtr<ABattleCharacter>> EnemyClasses;
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Battlers")
	TArray<FTransform> EnemyTransforms;
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Battlers", meta = (DisplayName = "Allies", EditCondition = "bGetAlliesFromPlayerController==false", EditConditionHides))
	TArray<TSoftClassPtr<ABattleCharacter>> AllyClasses;
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Battlers")
	TArray<FTransform> AllyTransforms;
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Battlers")
	bool bGetAlliesFromPlayerController = true;
	// 
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Camera", meta = (DisplayName = "Camera Blend Delay", ClampMin = "0", ToolTip = "Delay (in seconds) before starting a camera view blend.\n\nThis is automatically called when a Battler starts its input turn."))
	float CameraDelay = 0.8f;
	// ACTION
	TArray<UBattleAction*> Actions;
	TArray<int32> ActionOrder;
	int32 CurrentAction = -1;
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Action", meta = (ClampMin = "0"))
	float ActionDelay = 0.5f;

public:
	// BATTLE
	virtual void StartBattle(FName State = "None", float BlendTime = 2.0f) override;
	virtual void LoadBattlers_Implementation() override;
	virtual void EndBattle(FName State = "None", float BlendTime = 2.0f) override;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based", meta = (DisplayName = "Load and Spawn Battlers"))
	virtual void LoadSpawnBattlers(TArray<TSoftClassPtr<ABattleCharacter>> Battlers, UPARAM(ref) TArray<ABattleCharacter*>& BattlerGroup, TArray<FTransform> TransformGroup);
	// ACTION
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual bool IsActionValid(UBattleAction* Action) const;
	// GETTERS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void GetTargetsAllPossible(UPARAM(ref) UBattleAction*& Action, bool bUpdateCameraView = false);

	// NEW TURN-BASED COMBAT
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void StartTurn();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void LoadTurnActions(TArray<ABattleCharacter*> Characters, bool bIsAlly);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void AddTurnAction(UBattleAction* Action, bool bIgnoreSpeed = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void StartTurnAction();
};
