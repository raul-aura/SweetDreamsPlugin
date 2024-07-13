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
	
	void FindAllCameras();
	void UpdateCameras();

	// COMPONENTS
	TArray<UCameraComponent*> AuxCameras;
	UCameraComponent* PrimaryCamera;
	UCameraComponent* TargetCamera;

	// BLEND
	FTimerHandle BlendHandle;
	float BlendElapsedTime;
	float BlendTotalTime;

	// CAMERA PARAMS
	FVector StartLocation;
	FVector EndLocation;
	FRotator StartRotation;
	FRotator EndRotation;
	float StartFOV;
	float EndFOV;
	FPostProcessSettings EndPostProcess;

	// OPTIONS
	bool bCanTransferFov;
	bool bCanTransferPostProcess;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Star|Camera", meta = (ToolTip = "Overrides the primary camera that will be active and act as a view target. This camera will be the one receiving the settings from secondary cameras upon transfer."))
	void DefinePrimaryCamera(UCameraComponent* NewPrimaryCamera);

	//
/* @param CameraToMatch Camera Component to get settings and transfer to primary camera. Function won't happen if this input is null.
*/
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Star|Camera", meta = (ToolTip = "Transfer the properties of the camera to match to the primary camera."))
	void TransferCameraProperties(UCameraComponent* CameraToMatch, float blendTime = 1.0f, bool transferFieldOfView = true, bool transferPostProcess = true);
};
