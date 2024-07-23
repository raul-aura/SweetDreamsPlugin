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
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Camera", meta = (DisplayName = "Turn Input Start Delay", ClampMin = "0"))
	float TurnCameraDelay = 0.8f;
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Camera", meta = (DisplayName = "Focus on Targets Delay", ClampMin = "0"))
	float TargetCameraDelay = 0.0f;
	// ACTION
	TArray<UBattleAction*> Actions;
	TArray<int32> ActionOrder;
	int32 CurrentAction = -1;
	virtual void GetEnemyActions();
	UPROPERTY(EditAnywhere, Category = "Battle Manager|Action", meta = (ClampMin = "0"))
	float ActionDelay = 0.5f;
	//
	ABattleCharacter* AllyInput;
	int32 CurrentAllyIndex;
	TArray<ABattleCharacter*> AllyPossibleTargets;
	TArray<ABattleCharacter*> AllyTargets;
	UBattleAction* AllySelectedAction;

	// TURN-ACTION ORDER
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void InitializeTurn();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void EvaluateTurnStart();

public:
	// BATTLE
	virtual void StartBattle(FName State = "None", float BlendTime = 2.0f) override;
	virtual void LoadBattlers() override;
	virtual void EndBattle(FName State = "None", float BlendTime = 2.0f) override;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based", meta = (DisplayName = "Load and Spawn Battlers"))
	virtual void LoadSpawnBattlers(TArray<TSoftClassPtr<ABattleCharacter>> Battlers, TArray<ABattleCharacter*>& BattlerGroup, TArray<FTransform> TransformGroup);
	virtual void ChangeCameraView(ECameraFocus NewFocus, float BlendTime) override;
	// ACTION
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void AllyInputStart(ABattleCharacter* Ally = nullptr);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void AllySelectAction(UBattleAction* Action);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void AllySelectTargets(ABattleCharacter* Target);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void AllyActionConfirm();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void AddActionAuto(UBattleAction* Action, bool bAddAsLast = true);
	virtual void UpdatePossibleTargets(UBattleAction* Action);
	// GETTERS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual ABattleCharacter* GetCurrentInputAlly() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual TArray<ABattleCharacter*> GetPossibleTargets() const;
	// TURN-ACTION ORDER
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager|Turn-based")
	virtual void StartActionInOrder();


};
