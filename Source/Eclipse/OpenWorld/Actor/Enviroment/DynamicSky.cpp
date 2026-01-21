// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Actor/Enviroment/DynamicSky.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/VolumetricCloudComponent.h"

#include "Materials/MaterialInterface.h"

#include "OpenWorld/Data/Weather/WeatherData.h"



#pragma region Override Function

ADynamicSky::ADynamicSky()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create Components 
	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	SunLight->SetupAttachment(RootComponent);
	SunLight->bUseTemperature = true; 


	MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
	MoonLight->SetupAttachment(RootComponent);
	MoonLight->bUseTemperature = true; 

	ExponentialHightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHightFog"));
	ExponentialHightFog->SetupAttachment(RootComponent);

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("USkyLight"));
	SkyLight->SetupAttachment(RootComponent);
	SkyLight->SetRealTimeCapture(true);

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyAtmosphere->SetupAttachment(RootComponent);

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(RootComponent);
	PostProcess->bUnbound = 1; // infinite

	// Auto Exposure Off 
	PostProcess->Settings.bOverride_AutoExposureMinBrightness = 0;
	PostProcess->Settings.AutoExposureMaxBrightness = 0; 

	SkySphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphere"));
	SkySphere->SetupAttachment(RootComponent);
	SkySphere->SetWorldScale3D(FVector(100000.f, 100000.f, 100000.f));

	VolumetricCloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloud"));
	VolumetricCloud->SetupAttachment(RootComponent);
}

void ADynamicSky::BeginPlay()
{
	Super::BeginPlay();

	if (CreateSkySphereDyMaterialInst())
	{
		if (bCloudVisibility)
		{
			CreateCloudDyMaterialInst();
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("DynamicSky Failed to Create Skysphere's Dynamic Material Instacne at BeginPlay."))
	}
}

void ADynamicSky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTimeflows)
	{
		UpdateTimeOfDay(DeltaTime);
		UpdateSunMoonPosition();
	}

	const bool bIsNowDayTime = IsDayTime();

	if (bWasDayTime != bIsNowDayTime)
	{
		bWasDayTime = bIsNowDayTime;

		UpdateSkySettings();
	}
}

void ADynamicSky::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (CreateSkySphereDyMaterialInst())
	{
		if (bCloudVisibility)
		{
			CreateCloudDyMaterialInst();
		}

		
		UpdateSunMoonPosition();
		
		UpdateSkySettings();
	}
}

#pragma endregion

#pragma region Getter 

bool ADynamicSky::IsDayTime() const
{
	return (TimeOfDay > DawnTime + 0.2f && TimeOfDay < DuskTime - 0.2f);
}

bool ADynamicSky::IsNightTIme() const
{
	return !IsDayTime();
}

#pragma endregion


#pragma region Create Materials 
bool ADynamicSky::CreateSkySphereDyMaterialInst()
{
	if (!SkySphere)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkySphere is not exist.")); 

		return false;
	}

	SkySphere_DynamicMaterialInstance = SkySphere->CreateDynamicMaterialInstance(0, SkySphereMaterialInterface, TEXT("None"));

	if (SkySphere_DynamicMaterialInstance)
	{
		return true;
	}

	return false;
}

void ADynamicSky::CreateCloudDyMaterialInst()
{
	if (VolumetricCloud && CloudMaterialInstance)
	{
		// create dynamic material 
		VolumetricCloud_DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CloudMaterialInstance, this);
		VolumetricCloud->SetMaterial(VolumetricCloud_DynamicMaterialInstance);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Create Cloud Dynamic Material."));
	}
}
#pragma endregion

#pragma region Update Functions
void ADynamicSky::UpdateTimeOfDay(float DeltaTime)
{
	TimeOfDay += DeltaTime * TimeSpeed / 60.f;

	if (TimeOfDay >= 24.0f) TimeOfDay = 0.f;

	TimeOfDay = FMath::Clamp(TimeOfDay, 0.0f, 24.0f);

}

void ADynamicSky::UpdateSunMoonPosition()
{

	if (IsDayTime())
	{
		SunLight->SetVisibility(true);
		MoonLight->SetVisibility(false);

		float SunYaw = FMath::GetMappedRangeValueUnclamped
		(
			FVector2D(DawnTime, DuskTime - 0.2f),
			FVector2D(0.0f, -180.0f),
			TimeOfDay
		);



		SunLight->SetWorldRotation(FRotator(SunYaw, 0.f, 0.f));
	}
	else // Night Time 
	{
		SunLight->SetVisibility(false);
		MoonLight->SetVisibility(true);

		float MoonYaw;

		if (TimeOfDay >= DuskTime && TimeOfDay <= 24.0f) { // Dusk ~ MidNight 
			MoonYaw = FMath::GetMappedRangeValueUnclamped(
				FVector2D(DuskTime, 24.0f),
				FVector2D(0.0f, -90.0f),
				TimeOfDay
			);
		}
		else // MidNight ~ Dawn
		{
			MoonYaw = FMath::GetMappedRangeValueUnclamped(
				FVector2D(0.0f, DawnTime - 0.3f),
				FVector2D(-90.0f, -180.0f),
				TimeOfDay
			);

			// 달이 24시에 좌우 반전이 돼서 임시로 반전
			if(MoonRotation >= 0)
				MoonRotation *= -1; 
		}

		

		MoonLight->SetWorldRotation(FRotator(MoonYaw, 0.f,  0.f));

	}
}

void ADynamicSky::UpdateSkySettings()
{
	if (!WeatherData) return;

	const bool bIsDayTime = IsDayTime();

	
	if (bIsDayTime)
	{
		// Day Time Only 

		// off star and moon 
		bStarVisibility = false;
		bMoonVisibility = false;

		// Update Sun  
		UpdateSunSettings(); 
	}
	else
	{
		// Night Time Only 

		if (WeatherData->GetCurrentWeatherType() == EWeatherType::SUNNY)
		{
			bStarVisibility = true;
			bMoonVisibility = true;
		}

		// Update Moon 
		UpdateMoonSettings();
	}

	// Update Stars 
	UpdateStarSettings();

	// Update Sky Light 
	UpdateSkyLightSettings(bIsDayTime);

	// Update Sky Atmosphere 
	UpdateSkyAtmosphereSettings(bIsDayTime);

	// Update Expoential High Fog 
	UpdateExponentialHightFogSettings(bIsDayTime);
}


void ADynamicSky::UpdateSunSettings()
{
	// Get Sun Settings From WeatherData 
	const FDirectionalLightSettings SunLightSettings = WeatherData->GetDirectionalLightSettings(true);

	SunLight->SetIntensity(SunLightSettings.Intensity);
	SunLight->SetLightColor(SunLightSettings.LightColor);
	SunLight->SetLightSourceAngle(SunLightSettings.SourceAngle);
	SunLight->SetTemperature(SunLightSettings.Temperature);

	SkySphere_DynamicMaterialInstance->SetScalarParameterValue(FName("MoonVisibility"), 0.0f);
}

void ADynamicSky::UpdateMoonSettings()
{
	// Get Moon Settings From WeatherData
	const FDirectionalLightSettings MoonLightSettings = WeatherData->GetDirectionalLightSettings(false);

	// Set Moon Light Settings 
	MoonLight->SetIntensity(MoonLightSettings.Intensity);
	MoonLight->SetLightColor(MoonLightSettings.LightColor);
	MoonLight->SetLightSourceAngle(MoonLightSettings.SourceAngle);
	MoonLight->SetTemperature(MoonLightSettings.Temperature);

	if (!SkySphere_DynamicMaterialInstance) return;

	// Update Moon Settings in Skysphere Materials 
	if (bMoonVisibility)
	{
		SkySphere_DynamicMaterialInstance->SetScalarParameterValue(FName("MoonVisibility"), 1.0f);
		
		// Set Moon Tint 
		MoonTint.A = MoonBrightness;
		SkySphere_DynamicMaterialInstance->SetVectorParameterValue(FName("MoonTint"), MoonTint);

		// Set Moon Scale and Rotation 
		FVector4 MoonSettings(MoonScale, MoonRotation, 0, 0);
		SkySphere_DynamicMaterialInstance->SetVectorParameterValue(FName("MoonSettings"), MoonSettings);

	}
	else
	{
		SkySphere_DynamicMaterialInstance->SetScalarParameterValue(FName("MoonVisibility"), 0.0f);
	}
}

void ADynamicSky::UpdateSkyAtmosphereSettings(bool bIsDayTime)
{
	const FSkyAtmosphereSettings SkyAtmosphereSettings = WeatherData->GetSkyAtmosphereSettings(bIsDayTime);

	SkyAtmosphere->SetMultiScatteringFactor(SkyAtmosphereSettings.MultiScattering);
	SkyAtmosphere->SetRayleighScattering(SkyAtmosphereSettings.RayleightScattering);
	SkyAtmosphere->SetMieScatteringScale(SkyAtmosphereSettings.MieSactteringScale);
	SkyAtmosphere->SetMieAbsorptionScale(SkyAtmosphereSettings.MieAbsorptionScale);
	SkyAtmosphere->SetMieAnisotropy(SkyAtmosphereSettings.MieAnisotropy);
	SkyAtmosphere->SetAerialPespectiveViewDistanceScale(SkyAtmosphereSettings.AerialPerspectiveViewDistanceScale);

}

void ADynamicSky::UpdateSkyLightSettings(bool bIsDayTime)
{
	SkyLight->SetIntensity(WeatherData->GetSkyLightSettings(bIsDayTime).Intensity);
}

void ADynamicSky::UpdateExponentialHightFogSettings(bool bIsDayTime)
{
	const FExponentialHeightFogSettings ExponentialHightFogSettings = WeatherData->GetExponentialHeightFogSettings(bIsDayTime);

	ExponentialHightFog->SetVolumetricFog(ExponentialHightFogSettings.bUseVolumetricFog);
	ExponentialHightFog->SetVolumetricFogEmissive(ExponentialHightFogSettings.EmissiveColor);
	ExponentialHightFog->SetVolumetricFogExtinctionScale(ExponentialHightFogSettings.ExtinctionScale);
}

void ADynamicSky::UpdatePostProcessSettings(bool bIsDayTime)
{
}

void ADynamicSky::UpdateCloudSettings()
{
	if (bCloudVisibility)
	{
		VolumetricCloud->SetVisibility(true);

		if (VolumetricCloud_DynamicMaterialInstance)
		{
			// Set Cloud Albedo and Brightness  
			const float CloudBrightness = IsDayTime() ? DayTimeCloudeBrightness : NightTimeCloudeBrightness;
			FLinearColor CloudAlbedoVector = CloudAlbedo;
			CloudAlbedoVector.A = CloudBrightness;

			VolumetricCloud_DynamicMaterialInstance->SetVectorParameterValue(TEXT("CloudAlbedo"), CloudAlbedoVector);

			// Set Cloud Panning Speed 
			VolumetricCloud_DynamicMaterialInstance->SetScalarParameterValue(TEXT("PanningSpeed"), CloudPanningSpeed);
		}
		
	}
	else
	{
		VolumetricCloud->SetVisibility(false);
	}
	
}

void ADynamicSky::UpdateStarSettings()
{
	if (!SkySphere_DynamicMaterialInstance) return; 

	if (bStarVisibility)
	{
		// Set Visibility 
		SkySphere_DynamicMaterialInstance->SetScalarParameterValue(FName("StarVisibility"), 1.0f);

		// Set Texture, Noise Texture's Tiling and Panning  
		const FVector4 StarsSettings( StarTiling, StarPanningSpeed, StarNoiseTiling, StarNoisePanningSpeed);
		SkySphere_DynamicMaterialInstance->SetVectorParameterValue(FName("StarsSettings"), StarsSettings);

		// Set Star Tint and Brightness
		StarsTint.A = StarsBrightness; 
		SkySphere_DynamicMaterialInstance->SetVectorParameterValue(FName("StarsTint"), StarsTint);

	}
	else
	{
		SkySphere_DynamicMaterialInstance->SetScalarParameterValue(FName("StarVisibility"), 0.0f);
	}
}

#pragma endregion

