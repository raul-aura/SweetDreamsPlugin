// Fill out your copyright notice in the Description page of Project Settings.


#include "SweetDreamsSettings.h"

USweetDreamsSettings::USweetDreamsSettings()
{
	DreamStates.Add(ADreamStateAwake::StaticClass());
	DreamStates.Add(ADreamStateAwakeUI::StaticClass());
	DreamStates.Add(ADreamStateAsleep::StaticClass());
}