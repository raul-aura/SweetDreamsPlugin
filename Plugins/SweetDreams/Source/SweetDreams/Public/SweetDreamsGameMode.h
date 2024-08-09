// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SweetDreamsCore.h"
#include "SweetDreamsState.h"
#include "GameFramework/GameModeBase.h"
#include "SweetDreamsGameMode.generated.h"

class USweetDreamsCore;
class ASweetDreamsState;
struct FDreamUserSettings;

UCLASS()
class SWEETDREAMS_API ASweetDreamsGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	ASweetDreamsGameMode();

public:
	virtual void BeginPlay() override;

	// STATE
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Game Mode")
	ASweetDreamsState* GetStateByName(FName StateName) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Game Mode")
	ASweetDreamsState* GetStateByIndex(int32 Index) const;
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Game Mode")
	void StartState(ASweetDreamsState* State);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Game Mode", meta = (DisplayName = "Create And Add State", ReturnDisplayName = "Success"))
	bool CreateAddState(TSubclassOf<ASweetDreamsState> StateClass);

protected:
	UPROPERTY()
	USweetDreamsCore* Core = nullptr;

	// STATE
	UPROPERTY(BlueprintReadWrite, Category = "States")
	TArray<ASweetDreamsState*> DreamStates;
	UPROPERTY(BlueprintReadOnly, Category = "States")
	ASweetDreamsState* CurrentState;
};
