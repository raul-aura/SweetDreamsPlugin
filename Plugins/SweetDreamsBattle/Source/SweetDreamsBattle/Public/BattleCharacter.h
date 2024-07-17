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
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (DisplayName = "Battler Parameters"))
	UBattlerDataComponent* BattlerParams;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;

	// CHARACTER
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character")
	FString DisplayName = "Character"; // change to localizable text
	// BATTLE
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle", meta = (DisplayName = "Actions"))
	TArray<TSoftClassPtr<UBattleAction>> ActionClasses;
	UPROPERTY(BlueprintReadWrite, Category = "Battle")
	TArray<UBattleAction*> Actions;
	UFUNCTION(BlueprintCallable)
	virtual void CreateActions();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Character")
	virtual FString GetCharacterName() const;


	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle")
	virtual UBattlerDataComponent* GetBattlerParameters() const;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle")
	virtual UBattleAction* GetRandomAction() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle")
	virtual TArray<UBattleAction*> GetAllActions() const;
};
