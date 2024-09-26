// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractComponent.h"
#include "SweetDreamsInteractInterface.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bDrawDebugSphere)
	{
		FVector CharacterLocation = GetOwner()->GetActorLocation();
		DrawDebugSphere(GetWorld(), CharacterLocation, InteractRadius, 12, FColor::Green, false, -1.f, 0, 5.f);
	}
	if (bDrawDebugLine)
	{
		UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + (Camera->GetForwardVector() * InteractTraceDistance);
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 5.f);
	}
}

TArray<AActor*> UInteractComponent::FindInteractablesInRange()
{
	ActorsWithinRange.Empty();
	TArray<struct FOverlapResult> Overlaps;
	FVector ThisLocation = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	TArray<AActor*> ChildActors;
	GetOwner()->GetAllChildActors(ChildActors);
	QueryParams.AddIgnoredActors(ChildActors);
	bool bActorsFound = GetWorld()->OverlapMultiByObjectType(Overlaps, ThisLocation, FQuat::Identity, FCollisionObjectQueryParams::AllObjects, Sphere, QueryParams);
	for (FOverlapResult Result : Overlaps)
	{
		if (Result.Actor->Implements<USweetDreamsInteractInterface>())
		{
			ActorsWithinRange.Add(Result.GetActor());
		}
	}
	return ActorsWithinRange;
}

AActor* UInteractComponent::FindInteractableTraced()
{
	FHitResult HitResult;
	UCameraComponent* Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * InteractTraceDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	TArray<AActor*> ChildActors;
	GetOwner()->GetAllChildActors(ChildActors);
	QueryParams.AddIgnoredActors(ChildActors);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->Implements<USweetDreamsInteractInterface>())
		{
			ActorTraceHit = HitActor;
		}
		else
		{
			ActorTraceHit = nullptr;
		}
	}
	else
	{
		ActorTraceHit = nullptr;
	}
	return ActorTraceHit;
}

void UInteractComponent::Interact(bool bUseInRange, int32 RangeIndex)
{
	AActor* InteractActor = ActorTraceHit;
	if (bUseInRange)
	{
		if(ActorsWithinRange.IsValidIndex(RangeIndex)) InteractActor = ActorsWithinRange[RangeIndex];
	}
	if (InteractActor && ISweetDreamsInteractInterface::Execute_IsInteractable(InteractActor))
	{
		ISweetDreamsInteractInterface::Execute_OnInteract(InteractActor);
	}
}
