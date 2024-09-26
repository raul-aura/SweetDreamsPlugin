// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MulticameraComponent.h"
#include "SweetDreamsController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SweetDreamsCharacter.generated.h"

UENUM(BlueprintType)
enum class ECameraPerspective : uint8
{
	FIRST UMETA(DisplayName = "First-person"),
	THIRD UMETA(DisplayName = "Third-person"),
};

UCLASS()
class SWEETDREAMS_API ASweetDreamsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASweetDreamsCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Camera)
	ECameraPerspective CameraPerspective;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMulticameraComponent* MulticameraComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|Core|Character")
	float WalkSpeed = 600.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|Core|Character")
	float RunSpeed = 900.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams|Core|Character")
	float MaxRunTime = 0.f;
	UPROPERTY(BlueprintReadOnly)
	bool bIsRunning = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanRun = false;
	UPROPERTY(BlueprintReadOnly)
	float CurrentRunTime = 0.f;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle RunTimer;

	UPROPERTY(BlueprintReadWrite)
	bool bCanMove = true;
	UPROPERTY(BlueprintReadWrite)
	bool bCanMoveCamera = true;

public:	
	virtual void Tick(float DeltaTime) override;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	ASweetDreamsController* GetDreamController() const;
	//
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void Run();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void StopRunning();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraVertical(float Value, float Sensitivity = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraHorizontal(float Value, float Sensitivity = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void EvaluateCameraPerspective();
};
