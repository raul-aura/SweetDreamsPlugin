// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBasedBattleWidget.h"
#include "SweetDreamsBattleManager.h"
#include "TurnBasedBattle.generated.h"

class UBattleInputAction;

USTRUCT(BlueprintType)
struct FEnemyGroups
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSoftClassPtr<ABattleCharacter>> EnemyGroup;

	FEnemyGroups() {}
};

UCLASS()
class SWEETDREAMSBATTLE_API ATurnBasedBattle : public ASweetDreamsBattleManager
{
	GENERATED_BODY()
	
public:
	ATurnBasedBattle();

protected:
	virtual void BeginPlay() override;

	// UI
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UTurnBasedBattleWidget* TurnBattleWidget;

	// BATTLERS
	UPROPERTY(BlueprintReadOnly, Category = "Battlers")
	TArray<ABattleCharacter*> AllBattlers;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USceneComponent* AllyRoot; 
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	USceneComponent* EnemyRoot;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers", meta = (DisplayName = "Enemies"))
	TArray<TSoftClassPtr<ABattleCharacter>> EnemyClasses;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers")
	TArray<FTransform> EnemyTransforms;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers", meta = (DisplayName = "Allies", EditCondition = "bGetAlliesFromPlayerController==false", EditConditionHides))
	TArray<TSoftClassPtr<ABattleCharacter>> AllyClasses;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers")
	TArray<FTransform> AllyTransforms;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battlers")
	bool bGetAlliesFromPlayerController = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Groups")
	TArray<FEnemyGroups> EnemyGroups;
	// 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (ClampMin = "0"))
	float FirstTurnDelay = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera", meta = (DisplayName = "Camera Blend Delay", ClampMin = "0", ToolTip = "Delay (in seconds) before starting a camera view blend.\n\nThis is automatically called when a Battler starts its input turn."))
	float CameraDelay = 0.8f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	bool bAutoMoveCamera = true;
	// TURN
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn", meta = (ClampMin = "0"))
	int32 MaxTurns = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Turn")
	int32 CurrentTurn = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	bool bVictoryWhenMaxTurns = true;
	// ACTION
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	TArray<UBattleAction*> Actions;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Action")
	TSubclassOf<UBattleInputAction> InputActionClass;
	UPROPERTY(BlueprintReadOnly, Category = "Action")
	int32 CurrentAction = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Action")
	ABattleCharacter* CurrentActionBattler = nullptr;
	UPROPERTY(EditAnywhere, Category = "Action", meta = (ClampMin = "0"))
	float ActionDelay = 0.5f;

public:

	// BATTLE
	virtual void StartBattle(float BlendTime = 2.0f) override;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void EvaluateTransforms();
	virtual void LoadBattlers_Implementation() override;
	virtual bool EvaluateEndBattle_Implementation() override;
	virtual void EndBattle(float BlendTime = 2.0f) override;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager", meta = (DisplayName = "Load and Spawn Battlers"))
	virtual void LoadSpawnBattlers(TArray<TSoftClassPtr<ABattleCharacter>> Battlers, UPARAM(ref) TArray<ABattleCharacter*>& BattlerGroup, USceneComponent* BattlerRoot, TArray<FTransform> TransformGroup);
	virtual bool IsNameEqual(const TArray<ABattleCharacter*>& BattlerGroup, const FText& NameToCheck);
	
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void GetAlliesFromArray(TArray<TSoftClassPtr<ABattleCharacter>> NewAllies);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void GetEnemiesFromArray(TArray<TSoftClassPtr<ABattleCharacter>> NewEnemies);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void LoadEnemiesGroup(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual TArray<FTransform> GetRandomAllyTransforms() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual TArray<FTransform> GetRandomEnemyTransforms() const;
	UFUNCTION()
	virtual TArray<FTransform> GetRandomTransforms(const TArray<ABattleCharacter*>& BattlerGroup, const TArray<FTransform>& TransformGroup) const;
	// UI
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual UTurnBasedBattleWidget* GetTurnBattleWidget() const;
	// GETTERS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void GetTargetsAllPossible(UPARAM(ref) UBattleAction*& Action, bool bUpdateCameraView = false);
	UFUNCTION(BlueprintPure, Category = "Sweet Dreams|RPG|Turn Battle Manager", meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ATurnBasedBattle* FindActiveTurnBattle(const UObject* WorldContext, UPARAM(DisplayName="Battle Index") int32& BattleId);
	// TURNS
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void StartTurn();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void LoadTurnActions(TArray<ABattleCharacter*> Characters, bool bIsAlly);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void AddTurnAction(UBattleAction* Action, bool bIgnoreSpeed = false);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager", meta = (ReturnDisplayName = "Found and Removed"))
	virtual bool RemoveTurnAction(UBattleAction* Action);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	virtual void StartTurnAction();
	// ACTION
	// GetActionOfClass()
	// ChangeActionOrder()
	// GetAllActionsOfOwner()
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	void OnActionAdded(UBattleAction* Action);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	void OnActionRemoved(UBattleAction* Action);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Turn Battle Manager")
	void OnTurnStarted(int32 Turn);
};
