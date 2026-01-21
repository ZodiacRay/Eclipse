// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaGauge.generated.h"

class UImage;
UCLASS()
class ECLIPSE_API UStaminaGauge : public UUserWidget
{
	GENERATED_BODY()
protected:
	/** UMG에서 바인딩할 이미지 (게이지 머티리얼이 적용된 이미지) */
	UPROPERTY(meta = (BindWidget))
	UImage* GaugeImage;

	UPROPERTY(EditAnywhere)
	UMaterial* BaseMaterial;

	/** 다이나믹 머티리얼 */
	UPROPERTY()
	UMaterialInstanceDynamic* GaugeMaterial;

public:

	virtual void NativeConstruct() override;

	/** 스태미나 값을 0~1 범위로 갱신 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateGauge(float Ratio);

};
