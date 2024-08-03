// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsSettings.h"

USweetDreamsSettings::USweetDreamsSettings()
{
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintEnabled);
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintSaveOperations);
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintStateOperations);
	DebugFlags |= static_cast<uint8>(EDebugFlags::PrintStateCreated);
	DreamStates.Add(ADreamStateAwake::StaticClass());
	DreamStates.Add(ADreamStateAwakeUI::StaticClass());
	DreamStates.Add(ADreamStateAsleep::StaticClass());
}