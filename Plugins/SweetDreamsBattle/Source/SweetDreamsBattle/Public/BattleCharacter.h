// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsCharacter.h"
#include "BattlerDataComponent.h"
#include "BattleAction.h"
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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class UBoxComponent* BattleWorldArea;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	class UWidgetComponent* ParameterIndicator;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (DisplayName = "Battler Parameters"))
	UBattlerDataComponent* BattlerParams;

	// CHARACTER
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	FText CharacterName = FText::FromString(TEXT("Character"));
	// BATTLE
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle")
	TSubclassOf<class UBattleNumberWidget> DamageIndicatorClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle")
	TSubclassOf<class UBattlerParameterWidget> ParameterIndicatorClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle", meta = (DisplayName = "Indicator Forward Offset"))
	float IndicatorOffset = 50.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle")
	float IndicatorPadding = 15.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle", meta = (DisplayName = "Actions"))
	TArray<TSoftClassPtr<UBattleAction>> ActionClasses;
	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	TArray<UBattleAction*> Actions;
	UPROPERTY(BlueprintReadOnly, Category = "Battle")
	TArray<UBattleState*> States;
	UFUNCTION(BlueprintCallable)
	virtual void CreateActions();

	UPROPERTY(BlueprintReadWrite)
	bool bIsInBattle = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacked = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAbleToAct = true;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking = false;
	UPROPERTY(BlueprintReadWrite)
	bool bAttackOnCooldown = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Character")
	float AttackCooldown = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|RPG|Character")
	bool bAttackStopsMovement = true;

public:	
	virtual void Tick(float DeltaTime) override;
	// DATA
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual FText GetCharacterName() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void SetCharacterName(FText NewName);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual UBattlerDataComponent* GetBattlerParameters() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void SetIsInBatte(bool bNewIsBattle);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	bool GetIsAbleToAct() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	void SetIsAbleToAct(bool bNewAble);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnAttack();
	void OnAttack_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float OnDamageReceived(float Damage, bool bIsDamageMitigated);
	float OnDamageReceived_Implementation(float Damage, bool bIsDamageMitigated);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float MitigateDamage(float Damage);
	float MitigateDamage_Implementation(float Damage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IndicateDamage(float Value, bool bIsHealInstead = false);
	void IndicateDamage_Implementation(float Value, bool bIsHealInstead = false);
	UFUNCTION(BlueprintCallable)
	void RemoveDamageIndicator(UWidgetComponent* Component);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IndicateParameter(UBattlerDataComponent* BattlerParameters);
	void IndicateParameter_Implementation(UBattlerDataComponent* BattlerParameters);
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
	virtual void AddState(TSubclassOf<UBattleState> State, UObject* StateInstigator);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual void RemoveState(TSubclassOf<UBattleState> State);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual int32 RemoveAllStates();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Character")
	virtual TArray<UBattleState*> GetAllStates() const;
};
