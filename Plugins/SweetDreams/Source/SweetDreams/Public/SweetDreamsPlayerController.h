// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SweetDreamsPlayerController.generated.h"


UCLASS()
class SWEETDREAMS_API ASweetDreamsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void SetViewTargetWithBlend(class AActor* NewViewTarget, float BlendTime = 0, enum EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 0, bool bLockOutgoing = false) override;

	UFUNCTION(BlueprintCallable)
	class ASweetDreamsCharacter* GetDreamCharacter() const;
};
