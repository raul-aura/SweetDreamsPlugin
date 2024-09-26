// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "AIController.h"
#include "SweetDreamsController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDreamMovementCompleted, int32, MovementID);

UCLASS()
class SWEETDREAMS_API ASweetDreamsController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	FTransform OriginTransform;
	UPROPERTY(BlueprintReadOnly)
	FRotator OriginControlRotation;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentMovementID = -1;

public:
	UPROPERTY(BlueprintAssignable)
	FOnDreamMovementCompleted OnDreamMovementCompleted;

	UFUNCTION(BlueprintCallable)
	void SetOrigin(AActor* Actor);

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION(BlueprintCallable)
	void ResetPosition();
	UFUNCTION(BlueprintCallable)
	void ResetRotation();
	UFUNCTION(BlueprintCallable)
	void SetCurrentMovementID(int32 NewID);
};

