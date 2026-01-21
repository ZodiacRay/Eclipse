// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UStaminaGauge;

UCLASS()
class ECLIPSE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
private:
	FTimerHandle TextTimerHandle;
	FTimerHandle StaminaTimerHandle;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBox;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UStaminaGauge> StaminaGaugeClass;

	UPROPERTY()
	class UStaminaGauge* StaminaGaugeWidget;

public:
	UPlayerHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativePreConstruct() override;

public:

	UFUNCTION(BlueprintCallable)
	void ShowText(const FText& Message, const FLinearColor& Color, float AutoHideTime = 1.5f);
	
	UFUNCTION(BlueprintCallable)
	void ClearText();

	UFUNCTION(BlueprintCallable)
	void HideText();

public:
	void SetStamina(float Current, float Max);

	void ShowStaminaGauge();

	void HideStaminaGauge();
};
