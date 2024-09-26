// Fill out your copyright notice in the Description page of Project Settings.

#include "RenderActor.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/DirectionalLightComponent.h"

ARenderActor::ARenderActor()
{
	SkyLight = CreateDefaultSubobject<USkyLightComponent>("Skylight");
	SkyLight->SetupAttachment(RootComponent);
	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>("Sunlight");
	SunLight->SetupAttachment(SkyLight);
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>("Post Process");
	PostProcess->SetupAttachment(SkyLight);
	PostProcess->bUnbound = true;
	Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>("Fog");
	Fog->SetupAttachment(SkyLight);
}

void ARenderActor::QuickChangeRender(int32 Index)
{
	ChangePostProcessSettings(Index);
	QuickChangeSky(Index);
	QuickChangeSun(Index);
	QuickChangeFog(Index);
}

void ARenderActor::ChangePostProcessSettings(int32 Index)
{
	if (PostProcessSettings.IsValidIndex(Index))
	{
		PostProcess->Settings = PostProcessSettings[Index];
	}
}

void ARenderActor::QuickChangeSky(int32 Index)
{
	if (SkyQuickRender.IsValidIndex(Index))
	{
		SkyLight->Intensity = SkyQuickRender[Index].SkyIntensity;
		SkyLight->LightColor = SkyQuickRender[Index].SkyColor;
		SkyLight->LowerHemisphereColor = SkyQuickRender[Index].LowerHemisphereColor;
		SkyLight->bLowerHemisphereIsBlack = SkyQuickRender[Index].bUseLowerHemisphereColor;
	}
}

void ARenderActor::QuickChangeSun(int32 Index)
{
	if (SunQuickRender.IsValidIndex(Index))
	{
		SunLight->Intensity = SunQuickRender[Index].SunIntensity;
		SunLight->LightColor = SunQuickRender[Index].SunColor;
		SunLight->SetWorldRotation(SunQuickRender[Index].SunRotation);
	}
}

void ARenderActor::QuickChangeFog(int32 Index)
{
	if (FogQuickRender.IsValidIndex(Index))
	{
		Fog->FogDensity = FogQuickRender[Index].FogDensity;
		Fog->FogInscatteringColor = FogQuickRender[Index].FogColor;
		Fog->SetVolumetricFog(FogQuickRender[Index].bIsVolumetric);
	}
}
