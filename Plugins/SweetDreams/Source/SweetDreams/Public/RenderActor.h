// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RenderActor.generated.h"

USTRUCT(BlueprintType)
struct FFogQuickRender
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FogDensity = 0.02f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor FogColor = FLinearColor::Blue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsVolumetric = false;

	FFogQuickRender() {}
};

USTRUCT(BlueprintType)
struct FSunQuickRender
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SunIntensity = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor SunColor = FColor::White;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator SunRotation = FRotator::ZeroRotator;

	FSunQuickRender() {}
};

USTRUCT(BlueprintType)
struct FSkyQuickRender
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SkyIntensity = 0.02f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor SkyColor = FColor::White;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor LowerHemisphereColor = FLinearColor::Black;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseLowerHemisphereColor = true;

	FSkyQuickRender() {}
};

UCLASS()
class SWEETDREAMS_API ARenderActor : public AActor
{
	GENERATED_BODY()
	
	ARenderActor();
public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Sweet Dreams Render")
	class UPostProcessComponent* PostProcess;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Sweet Dreams Render")
	class UExponentialHeightFogComponent* Fog;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Sweet Dreams Render")
	class UDirectionalLightComponent* SunLight;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Sweet Dreams Render")
	class USkyLightComponent* SkyLight;

	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Render")
	void QuickChangeRender(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Render")
	void ChangePostProcessSettings(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Render")
	void QuickChangeSky(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Render")
	void QuickChangeSun(int32 Index);
	UFUNCTION(BlueprintCallable, Category = "Sweet Dreams|Core|Render")
	void QuickChangeFog(int32 Index);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Render")
	int32 CurrentQuickRender = 0;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Render")
	TArray<FPostProcessSettings> PostProcessSettings;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Render")
	TArray<FSkyQuickRender> SkyQuickRender;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Render")
	TArray<FSunQuickRender> SunQuickRender;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sweet Dreams Render")
	TArray<FFogQuickRender> FogQuickRender;
};

