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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserSettingsSignature, FDreamUserSettings, Settings);

UCLASS()
class SWEETDREAMS_API ASweetDreamsGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	// STATE
	
	UFUNCTION(BlueprintCallable)
	ASweetDreamsState* GetStateByName(FName StateName) const;
	ASweetDreamsState* GetStateByIndex(int32 Index) const;
	UFUNCTION(BlueprintCallable)
	void StartState(ASweetDreamsState* State);

	UPROPERTY(BlueprintAssignable)
	FGetUserSettingsSignature GetUserSettingsDelegate;

protected:
	USweetDreamsCore* Core;

	// STATE
	TArray<ASweetDreamsState*> DreamStates;
	ASweetDreamsState* CurrentState;
};
