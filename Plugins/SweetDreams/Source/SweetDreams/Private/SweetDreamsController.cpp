// Fill out your copyright notice in the Description page of Project Settings.

#include "SweetDreamsController.h"
#include "GameFramework/Character.h"
#include "SweetDreamsBPLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void ASweetDreamsController::BeginPlay()
{
	Super::BeginPlay();
}

void ASweetDreamsController::SetOrigin(AActor* Actor)
{
	OriginTransform = Actor->GetActorTransform();
}

void ASweetDreamsController::SetCurrentMovementID(int32 NewID)
{
	CurrentMovementID = NewID;
}

void ASweetDreamsController::ResetPosition() 
{
	FVector Start = GetPawn()->GetActorLocation();
	FVector End = OriginTransform.GetLocation();
	SetCurrentMovementID(-1);
	MoveToLocation(End);
}

void ASweetDreamsController::ResetRotation()
{
	FRotator Rotation = GetPawn()->GetActorRotation();
	FRotator EndRotation = OriginTransform.GetRotation().Rotator();
	GetPawn()->SetActorRotation(EndRotation);
}

void ASweetDreamsController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	OnDreamMovementCompleted.Broadcast(CurrentMovementID);
	OnDreamMovementCompleted.Clear();
	if (CurrentMovementID == -1)
	{
		ResetRotation();
	}
	CurrentMovementID = -1;
}
