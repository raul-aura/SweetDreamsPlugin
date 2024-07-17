// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleAction.h"
#include "BattleCharacter.h"
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
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* BattleRoot;

	// UI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interface")
	TSubclassOf<UUserWidget> BattleWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* BattleWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Sweet Dreams RPG|Battle")
	TArray<ABattleCharacter*> Enemies;
	UPROPERTY(BlueprintReadWrite, Category = "Sweet Dreams RPG|Battle")
	TArray<ABattleCharacter*> Allies;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle")
	virtual void StartBattle(FName State = "None", float BlendTime = 2.0f);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle")
	virtual void EndBattle(FName State = "None", float BlendTime = 2.0f);

	virtual void EvaluateEndBattle();

	bool IsBattleOngoing() const;

};
