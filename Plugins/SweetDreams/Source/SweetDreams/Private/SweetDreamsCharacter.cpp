// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsCharacter.h"
#include "SweetDreamsBPLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetStringLibrary.h"

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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->SetFieldOfView(80.0f);
	Camera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCanEverAffectNavigation(true);

	MulticameraComponent = CreateDefaultSubobject<UMulticameraComponent>("Multicamera Component");
	AddOwnedComponent(MulticameraComponent);

	AIControllerClass = ASweetDreamsController::StaticClass();
}

void ASweetDreamsCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (GetDreamController())
	{
		GetDreamController()->SetOrigin(this);
	}
}

void ASweetDreamsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ASweetDreamsController* ASweetDreamsCharacter::GetDreamController() const
{
	return Cast<ASweetDreamsController>(GetController());
}

void ASweetDreamsCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.0f && bCanMove)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASweetDreamsCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.0f && bCanMove)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

}

void ASweetDreamsCharacter::Run()
{
	if (!bCanMove) return;
	if (MaxRunTime > 0.f)
	{
		if (CurrentRunTime >= MaxRunTime) return;
	}
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	if (MaxRunTime <= 0.f) return;
	GetWorldTimerManager().SetTimer(RunTimer, [this]() {
		CurrentRunTime += GetWorld()->GetDeltaSeconds();
		if (CurrentRunTime >= MaxRunTime)
		{
			StopRunning();
		}
		}, GetWorld()->GetDeltaSeconds(), true);
}

void ASweetDreamsCharacter::StopRunning()
{
	bIsRunning = false;
	CurrentRunTime = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetWorldTimerManager().ClearTimer(RunTimer);
}

void ASweetDreamsCharacter::CameraVertical(float Value, float Sensitivity)
{
	if (!bCanMoveCamera) return;
	if (Sensitivity < 0.f) Sensitivity = 1.f;
	AddControllerPitchInput(Value * Sensitivity);
}

void ASweetDreamsCharacter::CameraHorizontal(float Value, float Sensitivity)
{
	if (!bCanMoveCamera) return;
	if (Sensitivity < 0.f) Sensitivity = 1.f;
	AddControllerYawInput(Value * Sensitivity);
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


