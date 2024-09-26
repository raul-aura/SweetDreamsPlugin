// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

UCLASS(ClassGroup = ("SweetDreams"), meta = (BlueprintSpawnableComponent))
class SWEETDREAMS_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact")
	TArray<AActor*> FindInteractablesInRange();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact")
	AActor* FindInteractableTraced();
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Interact")
	void Interact(bool bUseInRange, int32 RangeIndex);

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Interact")
	float InteractTraceDistance = 600.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Interact")
	float InteractRadius = 600.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Interact")
	bool bDrawDebugSphere = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Interact")
	bool bDrawDebugLine = false;

	UPROPERTY(BlueprintReadWrite, Category = "Sweet Dreams Interact")
	TArray<AActor*> ActorsWithinRange;
	UPROPERTY(BlueprintReadWrite, Category = "Sweet Dreams Interact")
	AActor* ActorTraceHit;
};

