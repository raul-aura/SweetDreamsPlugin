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

	FTimerHandle BattleTimer;

	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager")
	bool bIsBattleActive = false;

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* BattleCamera;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* BattleRoot;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	APlayerController* Player;

	// UI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Manager|UI")
	TSubclassOf<UUserWidget> BattleWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* BattleWidget;

	// BATLE
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager|Battlers", meta = (DisplayName = "Enemies References"))
	TArray<ABattleCharacter*> Enemies;
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager|Battlers", meta = (DisplayName = "Allies References"))
	TArray<ABattleCharacter*> Allies;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Manager|Camera", meta = (DisplayName = "Battler Camera Blend Time", ClampMin = "0"))
	float BattlerBlendTime = 1.0f;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager")
	virtual void StartBattle(FName State = "None", float BlendTime = 2.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager")
	virtual void EndBattle(FName State = "None", float BlendTime = 2.0f);
	virtual bool EvaluateEndBattle();
	bool IsBattleOngoing() const;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager")
	virtual void ChangeCameraFocus(AActor* NewFocus, float BlendTime = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams RPG|Battle Manager")
	virtual void ChangeCameraFocusDelayed(AActor* NewFocus, float BlendTime = 1.0f, float DelayTime = 1.0f);
};
