// Fill out your copyright notice in the Description page of Project Settings.


#include "MulticameraComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMulticameraComponent::UMulticameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMulticameraComponent::BeginPlay()
{
	Super::BeginPlay();
	FindCamera();
}

void UMulticameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMulticameraComponent::TransferCameraProperties(int32 CameraToMatch, float BlendTime)
{
	if (Locations.Num() == 0 || Rotations.Num() == 0)
	{
		return;
	}
	if (BlendTime <= 0.0f)
	{
		BlendTime = 0.01f;
	}
	StartLocation = ActiveCamera->GetRelativeLocation();
	StartRotation = ActiveCamera->GetRelativeRotation();
	EndLocation = Locations[CameraToMatch];
	EndRotation = Rotations[CameraToMatch];
	BlendElapsedTime = 0.0f;
	BlendTotalTime = BlendTime;
	GetOwner()->GetWorldTimerManager().SetTimer(BlendHandle, this, &UMulticameraComponent::UpdateCameras, 0.01f, true);
}

void UMulticameraComponent::SetActiveCamera(UCameraComponent* NewCamera)
{
	ActiveCamera = NewCamera;
}

void UMulticameraComponent::FindCamera() 
{
	ActiveCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
}

void UMulticameraComponent::UpdateCameras()
{
	if (BlendTotalTime <= 0.0f)
	{
		return;
	}
	BlendElapsedTime += GetWorld()->GetDeltaSeconds();
	float BlendAlpha = FMath::Clamp(BlendElapsedTime / BlendTotalTime, 0.0f, 1.0f);
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, BlendAlpha);
	FRotator NewRotation = FMath::Lerp(StartRotation, EndRotation, BlendAlpha);
	ActiveCamera->SetRelativeLocationAndRotation(NewLocation, NewRotation);
	if (BlendAlpha >= 1.0f) 
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(BlendHandle);
	}
}
