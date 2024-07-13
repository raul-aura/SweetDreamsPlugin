// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleAction.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "SweetDreams/Public/MulticameraComponent.h"
#include "GameFramework/Actor.h"
#include "SweetDreamsBattleManager.generated.h"

class USceneComponent;

UCLASS()
class SWEETDREAMSBATTLE_API ASweetDreamsBattleManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsBattleManager();

protected:
	virtual void BeginPlay() override;

	bool IsBattleActive = false;

	// COMPONENTS

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* BattleCamera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;

	// INTERFACE

	UPROPERTY(EditAnywhere, Category = "Interface")
	TSubclassOf<UUserWidget> BattleWidgetClass; //change class to an inherited UBattleWidget
	UUserWidget* BattleWidget;

	// BATTLERS

	USceneComponent* PlayerRoot; //migrate to BattleManagerTurn
	USceneComponent* EnemyRoot; //migrate to BattleManagerTurn
	TArray<USceneComponent*> PlayerLocations; //migrate to BattleManagerTurn
	TArray<USceneComponent*> EnemiesLocations; //migrate to BattleManagerTurn
	TArray<ACharacter*> Battlers; 
	/*
	* separete by ally or enemy
	* create function to EndBattle automatically when all enemies are dead.
	*/
	UPROPERTY(EditAnywhere, Category = "Battler")
	TSubclassOf<ACharacter> BattlerClass; //migrate to BattleManagerTurn, change class to an inherited ABattler

	// BATTLE

	TArray<UBattleAction> Actions;
	/*
	* create function that gets if all allies has at least 1 Action in the array to start Actions. (evaluate - for each ally->if ally action ok)
	* when starting Action, get the Action[x] where x = ActionOrder[0...Num]
	*/
	TArray<int32> ActionOrder;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams Battle|Battle Manager")
	virtual void StartBattle(UPARAM(DisplayName = "State To Initialize")FName State, float blendTime = 2.0f);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams Battle|Battle Manager")
	virtual void EndBattle(UPARAM(DisplayName = "State To Initialize")FName State, float blendTime = 2.0f);

	bool IsBattleOngoing() const;
};
