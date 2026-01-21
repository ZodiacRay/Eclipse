// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/UI/StaminaGauge.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UStaminaGauge::NativeConstruct()
{

	Super::NativeConstruct();

	if (GaugeImage)
	{
		if (BaseMaterial)
		{
			GaugeMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
			GaugeImage->SetBrushFromMaterial(GaugeMaterial);
		}
	}

}

void UStaminaGauge::UpdateGauge(float Ratio)
{
	if (GaugeMaterial)
	{
		GaugeMaterial->SetScalarParameterValue("Fill", FMath::Clamp(Ratio, 0.f, 1.f));
	}
}
