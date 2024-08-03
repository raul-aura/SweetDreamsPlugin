// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleAction.h"
#include "BattleCharacter.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "SweetDreams/Public/MulticameraComponent.h"
#include "GameFramework/Actor.h"
#include "LevelSequencePlayer.h"
#include "SweetDreamsBattleManager.generated.h"

class USceneComponent;

UENUM(BlueprintType)
enum class ECameraView : uint8
{
	AllBattlers = 0,
	Allies = 1,
	Enemies = 2,
	Self = 3,
};

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
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager")
	bool bIsVictorious = true;
	//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void OnBattleEnd(bool bBattleVictorious);

	// COMPONENTS
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	UCameraComponent* BattleCamera;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Components")
	USceneComponent* BattleRoot;
	UPROPERTY(BlueprintReadOnly, Category = "Battle Manager")
	APlayerController* Player;
	
	// UI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Manager|UI")
	TSubclassOf<UUserWidget> BattleWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* BattleWidget;

	// BATLE
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Battle Manager|Battlers", meta = (DisplayName = "Enemies References"))
	TArray<ABattleCharacter*> Enemies;
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager|Battlers", meta = (DisplayName = "Allies References"))
	TArray<ABattleCharacter*> Allies;
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager|Battlers")
	float EnemyDamage = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Battle Manager|Battlers")
	float AllyDamage = 0;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Battle Manager|Camera", meta = (DisplayName = "Battler Camera Blend Time", ClampMin = "0", Tooltip = "Time (in seconds) that the camera will blend between Battlers and between BattleManager camera to Battler camera."))
	float BattlerBlendTime = 1.0f;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void StartBattle(FName State = "None", float BlendTime = 2.0f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	void LoadBattlers();
	virtual void LoadBattlers_Implementation();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void EndBattle(FName State = "None", float BlendTime = 2.0f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager", meta = (ReturnDisplayName = "Battle Ended"))
	bool EvaluateEndBattle();
	virtual bool EvaluateEndBattle_Implementation();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	bool IsBattleOngoing() const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	bool IsBattleVictorious() const;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void ChangeCameraFocus(AActor* NewFocus, float BlendTime = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager", meta = (ToolTip = "When New View is Self and Actor Self Focus is valid, it will blend with the Camera Transform on Index 1 of the Actor instead."))
	virtual void ChangeCameraView(ECameraView NewView = ECameraView::AllBattlers, AActor* SelfFocus = nullptr, float BlendTime = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|RPG|Battle Manager")
	virtual void AddDamageToBattle(ABattleCharacter* DamageOwner, float Damage);
};
