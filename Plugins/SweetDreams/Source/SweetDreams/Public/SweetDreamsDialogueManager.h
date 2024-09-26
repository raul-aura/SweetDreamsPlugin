// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "MulticameraComponent.h"
#include "Engine/DataTable.h"
#include "SweetDreamsDialogueManager.generated.h"

USTRUCT(BlueprintType)
struct FSweetDreamsDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DialogueName = FText::FromString(TEXT("Name"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DialogueBody = FText::FromString(TEXT("Hello, I'm Name."));

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* DialogueImage = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0))
	int32 CameraID = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0))
	float CameraBlend = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* DialogueAudio = nullptr;

	FSweetDreamsDialogue() {}
};

USTRUCT(BlueprintType)
struct FSweetDreamsDialogueLog
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DialogueName = FText::FromString(TEXT("Name"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DialogueBody = FText::FromString(TEXT("Hello, I'm Name."));

	FSweetDreamsDialogueLog() {}
	FSweetDreamsDialogueLog(FText Name, FText Body)
		: DialogueName(Name),
		DialogueBody(Body)
	{}
};

UCLASS()
class SWEETDREAMS_API ASweetDreamsDialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASweetDreamsDialogueManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartDialogue(float TransitionDuration = 2.0f);
	UFUNCTION(BlueprintCallable)
	void UpdateDialogue();
	UFUNCTION(BlueprintCallable)
	void EndDialogue();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnDialogueStarted();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnDialogueChanged(FSweetDreamsDialogue Dialogue, int32 DialogueID);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnDialogueEnded();
	//
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsDialogueManager* GetActiveDialogue(const UObject* WorldContext);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContext", CallableWithoutWorldContext))
	static ASweetDreamsDialogueManager* FindDialogueByName(const UObject* WorldContext, FName Name);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentDialogueID() const;
	UFUNCTION(BlueprintCallable)
	FSweetDreamsDialogue GetCurrentDialogue() const;
	UFUNCTION(BlueprintCallable)
	TArray<FSweetDreamsDialogueLog> GetDialogueLog() const;
	UFUNCTION(BlueprintCallable)
	bool GetIsDialogueActive() const;

	// ANIMATED
	UFUNCTION(BlueprintCallable)
	void DisplayAnimatedDialogue(const FSweetDreamsDialogue& OriginalDialogue, FSweetDreamsDialogue& UpdatedDialogue);
	UPROPERTY()
	FTimerHandle DialogueTimer;
	UPROPERTY()
	int32 AnimatedIndex = 0;
	UPROPERTY()
	FString OriginalBody;
	UPROPERTY()
	FSweetDreamsDialogue AnimatedDialogue;
	UPROPERTY()
	FSweetDreamsDialogue UpdatedDialogueInstance;
	UFUNCTION()
	void DisplayNextLetter();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName DialogueName;

	UFUNCTION(BlueprintCallable)
	void AddDialogueToLog(int32 DialogueID);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	TArray<FSweetDreamsDialogue> Dialogues;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Dialogue")
	float EndTransitionDuration = 2.0f;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSweetDreamsDialogueLog> DialogueLog;
	UPROPERTY(BlueprintReadOnly)
	FSweetDreamsDialogue CurrentDialogue;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentDialogueID = -1;
	UPROPERTY(BlueprintReadOnly)
	APawn* OriginalPawn;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanRepeatDialogue = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bPossessThis = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bHideCharacter = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDialogueActive = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDialogueEnabled = true;
	//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animated Dialogue")
	float LetterDisplayRate = 0.1f;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;
};
