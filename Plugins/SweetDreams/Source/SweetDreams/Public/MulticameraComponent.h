// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "MulticameraComponent.generated.h"

USTRUCT(BlueprintType)
struct FCameraViews
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Sweet Dreams Multicamera", meta = (DisplayName = "Camera Location"))
	FVector Location;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Sweet Dreams Multicamera", meta = (DisplayName = "Camera Rotation"))
	FRotator Rotation;

	FCameraViews() {}
};

UCLASS(ClassGroup = ("SweetDreams"), meta = (BlueprintSpawnableComponent, ToolTip = "This component enables the management of multiple cameras on the same actor. Similar to a View Target Blend, but with internal cameras inside a single actor.\n\nUse the function SetNewCameraView() to transfer the settings of a secondary camera to the primary one."))
class SWEETDREAMS_API UMulticameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMulticameraComponent();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Category = "Sweet Dreams|Core|Multicamera")
	void FindCamera();
	UFUNCTION(Category = "Sweet Dreams|Core|Multicamera")
	void CameraBlend();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Multicamera")
	TArray<FCameraViews> CameraViews;

	// COMPONENTS
	UPROPERTY(BlueprintReadWrite, Category = "Sweet Dreams Multicamera")
	UCameraComponent* ActiveCamera;

	// BLEND
	FTimerHandle BlendHandle;
	float BlendElapsedTime;
	float BlendTotalTime;
	float AlphaMultiplier;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Multicamera")
	UCurveFloat* AlphaCurve;

	// CAMERA PARAMS
	FVector StartLocation;
	FVector EndLocation;
	FRotator StartRotation;
	FRotator EndRotation;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// @param CameraToMatch Camera Component to get settings and transfer to primary camera. Function won't happen if this input is null.
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Multicamera", meta = (ToolTip = "Transfer the properties of the camera to match to the primary camera."))
	virtual void SetNewCameraView(int32 CameraToMatch = 0, float BlendTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Multicamera", meta = (ToolTip = "Defines a new camera to be the current one. Ideal when you have multiple cameras."))
	virtual void SetActiveCamera(UCameraComponent* NewCamera);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Multicamera")
	virtual TArray<FCameraViews> GetAllPossibleViews() const;
};
