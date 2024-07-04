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
	DefinePrimaryCamera(nullptr);
	Super::BeginPlay();
}

void UMulticameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMulticameraComponent::TransferCameraProperties(UCameraComponent* CameraToMatch, float blendTime, bool transferFieldOfView, bool transferPostProcess)
{
	if (AuxCameras.Contains(PrimaryCamera))
	{
		return;
	}
	else if (!CameraToMatch)
	{
		return;
	}
	if (blendTime <= 0.0f)
	{
		blendTime = 0.01f;
	}
	TargetCamera = CameraToMatch;
	StartLocation = PrimaryCamera->GetRelativeLocation();
	StartRotation = PrimaryCamera->GetRelativeRotation();
	StartFOV = PrimaryCamera->FieldOfView;
	EndLocation = TargetCamera->GetRelativeLocation();
	EndRotation = TargetCamera->GetRelativeRotation();
	EndFOV = TargetCamera->FieldOfView;
	EndPostProcess = TargetCamera->PostProcessSettings;
	bCanTransferFov = transferFieldOfView;
	bCanTransferPostProcess = transferPostProcess;
	BlendElapsedTime = 0.0f;
	BlendTotalTime = blendTime;
	GetOwner()->GetWorldTimerManager().SetTimer(BlendHandle, this, &UMulticameraComponent::UpdateCameras, 0.01f, true);
}

void UMulticameraComponent::FindAllCameras() 
{
	GetOwner()->GetComponents<UCameraComponent>(AuxCameras);
	for (UCameraComponent* Camera : AuxCameras)
	{
		Camera->SetActive(false);
	}
}

void UMulticameraComponent::UpdateCameras()
{
	if (!PrimaryCamera || !TargetCamera || BlendTotalTime <= 0.0f)
	{
		return;
	}
	BlendElapsedTime += GetWorld()->GetDeltaSeconds();
	float BlendAlpha = FMath::Clamp(BlendElapsedTime / BlendTotalTime, 0.0f, 1.0f);
	FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, BlendAlpha);
	FRotator NewRotation = FMath::Lerp(StartRotation, EndRotation, BlendAlpha);
	float NewFOV = FMath::Lerp(StartFOV, EndFOV, BlendAlpha);
	PrimaryCamera->SetRelativeLocationAndRotation(NewLocation, NewRotation);
	if (bCanTransferFov)
	{
		PrimaryCamera->SetFieldOfView(NewFOV);
	}
	if (BlendAlpha >= 1.0f) 
	{
		if (bCanTransferPostProcess)
		{
			PrimaryCamera->PostProcessSettings = EndPostProcess;
		}
		GetOwner()->GetWorldTimerManager().ClearTimer(BlendHandle);
	}
}

void UMulticameraComponent::DefinePrimaryCamera(UCameraComponent* NewPrimaryCamera)
{
	FindAllCameras();
	if (!NewPrimaryCamera)
	{
		NewPrimaryCamera = AuxCameras[0];
	}
	AuxCameras.Remove(NewPrimaryCamera);
	PrimaryCamera = NewPrimaryCamera;
	PrimaryCamera->SetActive(true);
}
