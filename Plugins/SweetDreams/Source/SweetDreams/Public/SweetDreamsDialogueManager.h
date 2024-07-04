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
	UTexture2D* DialogueImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 0))
	int CameraID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FText> DialogueChoices; // maybe make a dictionary with what dialogue go to.

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* DialogueAudio;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueChangedSignature, FSweetDreamsDialogue, Dialogue, int, DialogueID);

USTRUCT(BlueprintType)
struct FSweetDreamsDialogueLog
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DialogueName = FText::FromString(TEXT("Name"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DialogueBody = FText::FromString(TEXT("Hello, I'm Name."));
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
	void StartDialogue(float TransitionDuration = 2.0f, bool bHideCharacter = true);

	UFUNCTION(BlueprintCallable)
	void EndDialogue(float TransitionDuration = 2.0f);

	UFUNCTION()
	int32 GetCurrentDialogue() const;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueChangedSignature OnDialogueChangedDelegate;

	//void OnDialogueChanged(FSweetDreamsDialogue Dialogue, int32 DialogueID);
protected:

	void ChangeDialogue();
	void FinishDialogueText();
	void AddDialogueToLog(int32 DialogueID);

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray <FSweetDreamsDialogue> Dialogue;
	TArray<FSweetDreamsDialogueLog> DialogueLog;
	FSweetDreamsDialogue CurrentDialogue;
	int32 CurrentDialogueID = 0;

	bool bCanRepeatDialogue = true;
	bool bCanChangeDialogue = true;
	bool bCurrentDialogueDone = false;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAudioComponent* AudioComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMulticameraComponent* MulticameraComponent;
};
