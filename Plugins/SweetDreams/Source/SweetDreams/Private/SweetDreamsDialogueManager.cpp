// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsDialogueManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

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

void ASweetDreamsDialogueManager::StartDialogue(float TransitionDuration)
{
	if (TransitionDuration <= 0.f) TransitionDuration = GetWorld()->GetDeltaSeconds();
	if (Dialogues.Num() == 0 || !bIsDialogueEnabled || bIsDialogueActive) return;
	if (bHideCharacter)
	{
		if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			Character->SetActorHiddenInGame(true);
		}
	}
	if (bPossessThis)
	{
		if (APlayerController* Player = GetWorld()->GetFirstPlayerController())
		{
			OriginalPawn = Player->GetPawn();
			Player->SetViewTargetWithBlend(this, TransitionDuration);
		}
	}
	bIsDialogueActive = true;
	UpdateDialogue();
	OnDialogueStarted();
}

void ASweetDreamsDialogueManager::UpdateDialogue()
{
	if (CurrentDialogueID >= Dialogues.Num() - 1)
	{
		EndDialogue();
		return;
	}
	CurrentDialogueID = FMath::Clamp(++CurrentDialogueID, 0, Dialogues.Num());
	CurrentDialogue = Dialogues[CurrentDialogueID];
	AddDialogueToLog(CurrentDialogueID);
	UGameplayStatics::PlaySound2D(this, CurrentDialogue.DialogueAudio);
	if (bPossessThis)
	{
		MulticameraComponent->SetNewCameraView(CurrentDialogue.CameraID, CurrentDialogue.CameraBlend);
	}
	OnDialogueChanged(CurrentDialogue, CurrentDialogueID);
}

void ASweetDreamsDialogueManager::EndDialogue()
{
	float TransitionDuration = EndTransitionDuration;
	if (TransitionDuration <= 0.f) TransitionDuration = GetWorld()->GetDeltaSeconds();
	bIsDialogueActive = false;
	CurrentDialogue = FSweetDreamsDialogue();
	DialogueLog.Empty();
	CurrentDialogueID = -1;
	if (bPossessThis)
	{
		if (APlayerController* Player = GetWorld()->GetFirstPlayerController())
		{
			Player->SetViewTargetWithBlend(OriginalPawn, TransitionDuration);
		}
	}
	if (!bCanRepeatDialogue)
	{
		bIsDialogueEnabled = false;
	}
	if (bHideCharacter)
	{
		if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			Character->SetActorHiddenInGame(false);
		}
	}
	OnDialogueEnded();
}

ASweetDreamsDialogueManager* ASweetDreamsDialogueManager::GetActiveDialogue(const UObject* WorldContext)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")))
	{
		return nullptr;
	}
	TArray<AActor*> Dialogues;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsDialogueManager::StaticClass(), Dialogues);
	for (AActor* Actor : Dialogues)
	{
		ASweetDreamsDialogueManager* Dialogue = Cast<ASweetDreamsDialogueManager>(Actor);
		if (Dialogue && Dialogue->bIsDialogueActive)
		{
			return Dialogue;
		}
	}
	return nullptr;
}

ASweetDreamsDialogueManager* ASweetDreamsDialogueManager::FindDialogueByName(const UObject* WorldContext, FName Name)
{
	if (!ensureAlwaysMsgf(IsValid(WorldContext), TEXT("World Context was not valid.")) || Name.IsNone())
	{
		return nullptr;
	}
	TArray<AActor*> Dialogues;
	UGameplayStatics::GetAllActorsOfClass(WorldContext, ASweetDreamsDialogueManager::StaticClass(), Dialogues);
	for (AActor* Actor : Dialogues)
	{
		ASweetDreamsDialogueManager* Dialogue = Cast<ASweetDreamsDialogueManager>(Actor);
		if (Dialogue && Dialogue->DialogueName.IsEqual(Name))
		{
			return Dialogue;
		}
	}
	return nullptr;
}

int32 ASweetDreamsDialogueManager::GetCurrentDialogueID() const
{
	return CurrentDialogueID;
}

FSweetDreamsDialogue ASweetDreamsDialogueManager::GetCurrentDialogue() const
{
	return CurrentDialogue;
}

TArray<FSweetDreamsDialogueLog> ASweetDreamsDialogueManager::GetDialogueLog() const
{
	return DialogueLog;
}

bool ASweetDreamsDialogueManager::GetIsDialogueActive() const
{
	return bIsDialogueActive;
}

void ASweetDreamsDialogueManager::AddDialogueToLog(int32 DialogueID)
{
	FText LogText = Dialogues[DialogueID].DialogueBody;
	FText LogName = Dialogues[DialogueID].DialogueName;
	FSweetDreamsDialogueLog NewLog = FSweetDreamsDialogueLog(LogName, LogText);
	DialogueLog.Add(NewLog);
}

void ASweetDreamsDialogueManager::DisplayAnimatedDialogue(const FSweetDreamsDialogue& OriginalDialogue, FSweetDreamsDialogue& UpdatedDialogue)
{
	OriginalBody = OriginalDialogue.DialogueBody.ToString();
	if (OriginalBody.IsEmpty()) return;
	AnimatedIndex = 0;
	AnimatedDialogue = OriginalDialogue;
	AnimatedDialogue.DialogueBody = FText::GetEmpty();
	UpdatedDialogueInstance = AnimatedDialogue;
	UpdatedDialogue = UpdatedDialogueInstance;
	GetWorldTimerManager().SetTimer(DialogueTimer, this, &ASweetDreamsDialogueManager::DisplayNextLetter, LetterDisplayRate, true);
}

void ASweetDreamsDialogueManager::DisplayNextLetter()
{
	if (AnimatedIndex < OriginalBody.Len())
	{
		FString AnimatedBody = AnimatedDialogue.DialogueBody.ToString();
		AnimatedBody.AppendChar(OriginalBody[AnimatedIndex]);
		AnimatedDialogue.DialogueBody = FText::FromString(AnimatedBody);
		UpdatedDialogueInstance = AnimatedDialogue;
		AnimatedIndex++;
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DialogueTimer);
	}
}



