// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "MulticameraComponent.generated.h"

UCLASS(ClassGroup = ("SweetDreams"), meta = (BlueprintSpawnableComponent, ToolTip = "This component enables the management of multiple cameras on the same actor. Similar to a View Target Blend, but with internal cameras inside a single actor.\n\nUse the function TransferCameraProperties() to transfer the settings of a secondary camera to the primary one."))
class SWEETDREAMS_API UMulticameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMulticameraComponent();

protected:
	virtual void BeginPlay() override;
	
	void FindCamera();
	void UpdateCameras();

	UPROPERTY(EditAnywhere, Category = "Cameras", meta = (DisplayName = "Camera Locations"))
	TArray<FVector> Locations;
	UPROPERTY(EditAnywhere, Category = "Cameras", meta = (DisplayName = "Camera Rotations"))
	TArray<FRotator> Rotations;

	// COMPONENTS
	UCameraComponent* ActiveCamera;

	// BLEND
	FTimerHandle BlendHandle;
	float BlendElapsedTime;
	float BlendTotalTime;

	// CAMERA PARAMS
	FVector StartLocation;
	FVector EndLocation;
	FRotator StartRotation;
	FRotator EndRotation;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// @param CameraToMatch Camera Component to get settings and transfer to primary camera. Function won't happen if this input is null.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Star|Camera", meta = (ToolTip = "Transfer the properties of the camera to match to the primary camera."))
	void TransferCameraProperties(int32 CameraToMatch = 0, float BlendTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Star|Camera", meta = (ToolTip = "Defines a new camera to be the current one. Ideal when you have multiple cameras."))
	void SetActiveCamera(UCameraComponent* NewCamera);
};
