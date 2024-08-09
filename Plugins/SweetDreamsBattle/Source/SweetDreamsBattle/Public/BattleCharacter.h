// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BattlerDataComponent.h"
#include "BattleAction.h"
#include "SweetDreams/Public/MulticameraComponent.h"
#include "GameFramework/Character.h"
#include "BattleCharacter.generated.h"

UCLASS()
class SWEETDREAMSBATTLE_API ABattleCharacter : public  ASweetDreamsCharacter
{
	GENERATED_BODY()

public:
	ABattleCharacter();

protected:
	virtual void BeginPlay() override;

	// COMPONENTS
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (DisplayName = "Battler Parameters"))
	UBattlerDataComponent* BattlerParams;

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
	// DATA
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual FText GetCharacterName() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void SetCharacterName(FText NewName);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual UBattlerDataComponent* GetBattlerParameters() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float OnDamageReceived(float Damage, bool bIsDamageMitigated);
	float OnDamageReceived_Implementation(float Damage, bool bIsDamageMitigated);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnKilled();
	void OnKilled_Implementation();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnRevived();
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
