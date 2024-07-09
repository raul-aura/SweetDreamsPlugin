// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsDialogueManager.h"

ASweetDreamsDialogueManager::ASweetDreamsDialogueManager()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* DialogueManager = CreateDefaultSubobject<USceneComponent>(TEXT("Dialogue Manager"));
	DialogueManager->SetupAttachment(RootComponent);

	USceneComponent* CameraGroup = CreateDefaultSubobject<USceneComponent>(TEXT("Camera Root"));
	CameraGroup->SetupAttachment(DialogueManager);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Primary Camera"));
	CameraComponent->SetupAttachment(CameraGroup);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);
}

void ASweetDreamsDialogueManager::BeginPlay()
{
	Super::BeginPlay();
}

void ASweetDreamsDialogueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASweetDreamsDialogueManager::StartDialogue(float TransitionDuration, bool bHideCharacter)
{
	if (Dialogue.Num() == 0)
	{
		return;
	}
	if (bCanChangeDialogue)
	{
		if (CurrentDialogueID >= Dialogue.Num())
		{
			EndDialogue();
			return;
		}
		ChangeDialogue();
	}
	else
	{
		FinishDialogueText();
	}
}

void ASweetDreamsDialogueManager::EndDialogue(float TransitionDuration)
{
	DialogueLog.Empty();
	CurrentDialogueID = 0;
	if (!bCanRepeatDialogue)
	{
		// disable interaction
	}
}

int32 ASweetDreamsDialogueManager::GetCurrentDialogue() const
{
	return CurrentDialogueID;
}

void ASweetDreamsDialogueManager::ChangeDialogue()
{
	CurrentDialogue = Dialogue[CurrentDialogueID];
	OnDialogueChangedDelegate.Broadcast(CurrentDialogue, GetCurrentDialogue());
}

void ASweetDreamsDialogueManager::FinishDialogueText()
{
	// update current dialogue text to be all letters.
	bCanChangeDialogue = true;
	AddDialogueToLog(CurrentDialogueID);
	CurrentDialogueID++;
}

void ASweetDreamsDialogueManager::AddDialogueToLog(int32 DialogueID)
{
	FText LogText = Dialogue[DialogueID].DialogueBody;
	FText LogName = Dialogue[DialogueID].DialogueName;
}
