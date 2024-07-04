// Copyright Epic Games, Inc. All Rights Reserved.

#include "SweetDreamsBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SweetDreams.h"

USweetDreamsCore* USweetDreamsBPLibrary::CoreLib = nullptr;

USweetDreamsBPLibrary::USweetDreamsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

USweetDreamsCore* USweetDreamsBPLibrary::GetSweetDreamsCore(const UObject* WorldContextObject)
{
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			CoreLib = GameInstance->GetSubsystem<USweetDreamsCore>();
		}
	}
	return CoreLib;
}

void USweetDreamsBPLibrary::PrintDream(UObject* DreamOrigin, FString Dream, EPrintType Severity, float Duration)
{
	GetSweetDreamsCore(DreamOrigin)->PrintDream(DreamOrigin, Dream, Severity, Duration);
}
