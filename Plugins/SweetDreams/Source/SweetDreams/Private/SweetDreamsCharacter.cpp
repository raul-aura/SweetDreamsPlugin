// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "MulticameraComponent.h"

ASweetDreamsCharacter::ASweetDreamsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 1.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->SetFieldOfView(80.0f);
	Camera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCanEverAffectNavigation(true);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>(TEXT("Multicamera Component"));
	AddOwnedComponent(MulticameraComponent);
}

void ASweetDreamsCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASweetDreamsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASweetDreamsCharacter::MoveForward(float Value)
{
	if (!Controller && Value != 0.0f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, Value);
	}
}

void ASweetDreamsCharacter::MoveRight(float Value)
{
	if (!Controller && Value != 0.0f)
	{
		FVector Right = GetActorRightVector();
		AddMovementInput(Right, Value);
	}
}

void ASweetDreamsCharacter::CameraUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ASweetDreamsCharacter::CameraRight(float Value)
{
	AddControllerYawInput(Value);
}

void ASweetDreamsCharacter::EvaluateCameraPerspective()
{
	float NewLength = 0.0f;
	FVector NewOffset = FVector();
	switch (CameraPerspective)
	{
	case ECameraPerspective::FIRST:
		NewLength = 0.0f;
		NewOffset = FVector(0.0f, 25.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
		break;
	case ECameraPerspective::THIRD:
		NewLength = 350.0f;
		NewOffset = FVector(25.0f, 0.0f, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - 20.0f);
		break;
	default:
		break;
	}
	CameraBoom->TargetArmLength = NewLength;
	CameraBoom->SocketOffset = NewOffset;
}


