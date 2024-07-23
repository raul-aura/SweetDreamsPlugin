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

void UMulticameraComponent::SetNewCameraView(int32 CameraToMatch, float BlendTime)
{
	if (CameraViews.Num() == 0 || !CameraViews.IsValidIndex(CameraToMatch))
	{
		return;
	}
	if (BlendTime <= 0.0f)
	{
		BlendTime = GetWorld()->GetDeltaSeconds();
	}
	StartLocation = ActiveCamera->GetRelativeLocation();
	StartRotation = ActiveCamera->GetRelativeRotation();
	EndLocation = CameraViews[CameraToMatch].Location;
	EndRotation = CameraViews[CameraToMatch].Rotation;
	BlendElapsedTime = 0.0f;
	BlendTotalTime = BlendTime;
	AlphaMultiplier = 1.0f;
	GetOwner()->GetWorldTimerManager().SetTimer(BlendHandle, this, &UMulticameraComponent::CameraBlend, GetWorld()->GetDeltaSeconds(), true);
}

void UMulticameraComponent::SetActiveCamera(UCameraComponent* NewCamera)
{
	ActiveCamera = NewCamera;
}

TArray<FCameraViews> UMulticameraComponent::GetAllPossibleViews() const
{
	return CameraViews;
}

void UMulticameraComponent::FindCamera() 
{
	ActiveCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
}

void UMulticameraComponent::CameraBlend()
{
	if (BlendTotalTime <= 0.0f)
	{
		return;
	}
	BlendElapsedTime += GetWorld()->GetDeltaSeconds();
	if (AlphaCurve)
	{
		if (AlphaCurve->FloatCurve.GetNumKeys() > 0)
		{
			AlphaMultiplier = AlphaCurve->GetFloatValue(BlendElapsedTime);
		}
	}
	float BlendAlpha = FMath::Clamp(BlendElapsedTime / BlendTotalTime, 0.0f, 1.0f);
	BlendAlpha *= AlphaMultiplier;
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, BlendAlpha);
	FRotator NewRotation = FMath::Lerp(StartRotation, EndRotation, BlendAlpha);
	ActiveCamera->SetRelativeLocationAndRotation(NewLocation, NewRotation);
	if (BlendAlpha >= 1.0f) 
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(BlendHandle);
	}
}
