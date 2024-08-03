// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BattlerDataComponent.h"
#include "BattleAction.h"
#include "SweetDreams/Public/MulticameraComponent.h"
#include "GameFramework/Character.h"
#include "BattleCharacter.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API ABattleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABattleCharacter();

protected:
	virtual void BeginPlay() override;

	// COMPONENTS
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	USpringArmComponent* CameraBoom;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components", meta = (DisplayName = "Battler Parameters"))
	UBattlerDataComponent* BattlerParams;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;

	// CHARACTER
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	FText CharacterName = FText::FromString(TEXT("Character"));
	// BATTLE
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle", meta = (DisplayName = "Actions"))
	TArray<TSoftClassPtr<UBattleAction>> ActionClasses;
	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	TArray<UBattleAction*> Actions;
	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	TArray<UBattleState*> States;
	UFUNCTION(BlueprintCallable)
	virtual void CreateActions();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// DATA
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual FText GetCharacterName() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void SetCharacterName(FText NewName);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual UBattlerDataComponent* GetBattlerParameters() const;
	// ACTION
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual UBattleAction* GetRandomAction() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual TArray<UBattleAction*> GetAllActions() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void UpdateActionsCooldown();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void ResetActions();
	// STATE
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void AddState(TSubclassOf<UBattleState> State, ABattleCharacter* StateInstigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void RemoveState(TSubclassOf<UBattleState> State);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual int32 RemoveAllStates();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual TArray<UBattleState*> GetAllStates() const;
	// MOVEMENT
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void MoveToLocation(FVector Location);
};
