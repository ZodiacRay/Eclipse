// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicSky.generated.h"



UCLASS()
class ECLIPSE_API ADynamicSky : public AActor
{
	GENERATED_BODY()

#pragma region Component
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UDirectionalLightComponent* SunLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UDirectionalLightComponent* MoonLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UExponentialHeightFogComponent* ExponentialHightFog;

	/** */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USkyAtmosphereComponent* SkyAtmosphere;

	/** */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UPostProcessComponent* PostProcess;

	/** */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* SkySphere;

	/** */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USkyLightComponent* SkyLight;

	/** */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UVolumetricCloudComponent* VolumetricCloud;
#pragma endregion

#pragma region Materials
protected:
	// Materials 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material | SkySphere")
	UMaterialInstance* SkySphereMaterialInterface;

	// UMaterialInterface* SkySphereMaterialInterface;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material | Cloud")
	UMaterialInstance* CloudMaterialInstance;

	// Dynamic Material Instances 
	UPROPERTY()
	UMaterialInstanceDynamic* SkySphere_DynamicMaterialInstance;

	UPROPERTY()
	UMaterialInstanceDynamic* VolumetricCloud_DynamicMaterialInstance;
#pragma endregion

#pragma region Time 
protected:
	// Time Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	bool bTimeflows = true; // 시간이 흐르는지 여부 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 0.f, ClampMax = 24.f))
	float TimeOfDay = 11.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float DawnTime = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float DuskTime = 18.0f;

private:
	bool bWasDayTime = false;  // 이전 프레임의 상태 저장

public:
	FORCEINLINE float GetTimeOfDay() const { return TimeOfDay; }

	FORCEINLINE bool IsDayTime() const;
	FORCEINLINE bool IsNightTIme() const;

	UFUNCTION(BlueprintCallable)
	void SetTimeOfDay(float nextTime) { TimeOfDay = nextTime; }

#pragma endregion


#pragma region Star 
protected:
	// Stars Value  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star")
	bool bStarVisibility = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star")
	float StarTiling = 3.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star")
	float StarPanningSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star")
	float StarNoiseTiling = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star")
	float StarNoisePanningSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star")
	FLinearColor StarsTint = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star")
	float StarsBrightness = 1.0f;
#pragma endregion

#pragma region Moon
protected:
	// Moon Value
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon | Visibility")
	bool bMoonVisibility = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon | Offset ")
	float MoonScale = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon | Offset")
	float MoonRotation = 0.f; // Radian 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon | Tint")
	FLinearColor MoonTint = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moon | Brightness")
	float MoonBrightness = 1.f; 
#pragma endregion

#pragma region Cloud 
protected:
	// Cloud Values
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud | Visibility")
	bool bCloudVisibility = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud | Brightness")
	float DayTimeCloudeBrightness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud | Brightness")
	float NightTimeCloudeBrightness = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud | Panning")
	float CloudPanningSpeed = 1.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloud | Abledo")
	FLinearColor CloudAlbedo = FLinearColor::White;

#pragma endregion

#pragma region Data
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather Settings")
	class UWeatherData* WeatherData;

#pragma endregion

#pragma region Default Function
public:	
	ADynamicSky();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;
#pragma endregion

#pragma region Init Functions 
private:
	bool CreateSkySphereDyMaterialInst();

	void CreateCloudDyMaterialInst(); 

#pragma endregion

#pragma region Update Functions
protected:
	// Update Time Of Day 
	void UpdateTimeOfDay(float DeltaTime);

	// Update directional Light's Position 
	void UpdateSunMoonPosition();

	// Update Component's Value and Material's Parameters 
	void UpdateSkySettings();

private:
	void UpdateSunSettings(); 

	void UpdateMoonSettings();

	void UpdateSkyAtmosphereSettings(bool bIsDayTime);

	void UpdateSkyLightSettings(bool bIsDayTime);

	void UpdateExponentialHightFogSettings(bool bIsDayTime);

	void UpdatePostProcessSettings(bool bIsDayTime);
	
	void UpdateCloudSettings();

	void UpdateStarSettings();


#pragma endregion
};
