// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeatherData.generated.h"


UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	SUNNY		UMETA(DisplayName = "Sunny"),
	CLOUDY		UMETA(DisplayName = "Cloudy"),
	SNOWY		UMETA(DisplayName = "Snowy"),
	RAINNY		UMETA(DisplayName = "Rainy"),
};


USTRUCT(BlueprintType)
struct FDirectionalLightSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor = FLinearColor::White; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SourceAngle = 0.537f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Temperature = 6500.f;
};

USTRUCT(BlueprintType)
struct FSkyLightSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity = 1.f;
};


USTRUCT(BlueprintType)
struct FSkyAtmosphereSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MultiScattering = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor RayleightScattering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MieSactteringScale = 0.003996f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MieAbsorptionScale = 0.000444f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MieAnisotropy = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AerialPerspectiveViewDistanceScale = 1.0f;
};

USTRUCT(BlueprintType)
struct FExponentialHeightFogSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseVolumetricFog = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor EmissiveColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExtinctionScale = 0.5f;
};

UCLASS()
class ECLIPSE_API UWeatherData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Type")
	EWeatherType CurrentWeatherType;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time | Directional Light")
	FDirectionalLightSettings DayTimeDirectionalLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time | Sky Light")
	FSkyLightSettings DayTimeSkyLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time | Sky Atmosphere")
	FSkyAtmosphereSettings DayTimeSkyAtmosphereSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Day Time | Exponential Hight Fog")
	FExponentialHeightFogSettings DayTimeExponentialHeightFogSettings;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time | Directional Light")
	FDirectionalLightSettings NightTimeDirectionalLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time | Sky Light")
	FSkyLightSettings NightTimeSkyLightSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time | Sky Atmosphere")
	FSkyAtmosphereSettings NightTimeSkyAtmosphereSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Night Time | Exponential Hight Fog")
	FExponentialHeightFogSettings NightTimeExponentialHeightFogSettings;


public:
	FORCEINLINE const EWeatherType GetCurrentWeatherType() const { return CurrentWeatherType; }

	FORCEINLINE const FDirectionalLightSettings& GetDirectionalLightSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeDirectionalLightSettings : NightTimeDirectionalLightSettings; }
	FORCEINLINE const FSkyLightSettings& GetSkyLightSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeSkyLightSettings : NightTimeSkyLightSettings; }

	FORCEINLINE const FSkyAtmosphereSettings& GetSkyAtmosphereSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeSkyAtmosphereSettings : NightTimeSkyAtmosphereSettings; }
	FORCEINLINE const FExponentialHeightFogSettings& GetExponentialHeightFogSettings(bool bIsDayTime) const { return bIsDayTime ? DayTimeExponentialHeightFogSettings : NightTimeExponentialHeightFogSettings; }


};
