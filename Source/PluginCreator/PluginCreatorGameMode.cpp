// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginCreatorGameMode.h"
#include "PluginCreatorCharacter.h"
#include "UObject/ConstructorHelpers.h"

APluginCreatorGameMode::APluginCreatorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
