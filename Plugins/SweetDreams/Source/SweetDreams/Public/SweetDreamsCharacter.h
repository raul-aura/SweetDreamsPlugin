// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Camera)
	ECameraPerspective CameraPerspective;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UMulticameraComponent* MulticameraComponent;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraUp(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void CameraRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Character")
	void EvaluateCameraPerspective();

};
