// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/UI/PlayerHUD.h"
#include "Components/TextBlock.h"
#include "OpenWorld/UI/StaminaGauge.h"




UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		// Bind Attribute 
		
	}
}

void UPlayerHUD::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{

	}
}

void UPlayerHUD::ShowText(const FText& Message, const FLinearColor& Color, float AutoHideTime)
{
	if (TextBox)
	{
		TextBox->SetText(Message);
		TextBox->SetColorAndOpacity(FSlateColor(Color));
		TextBox->SetVisibility(ESlateVisibility::Visible);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(TextTimerHandle);
			World->GetTimerManager().SetTimer(
				TextTimerHandle,
				this,
				&UPlayerHUD::HideText,
				AutoHideTime,
				false);
		}
	}
}

void UPlayerHUD::ClearText()
{
	TextBox->SetText(FText::GetEmpty());
}

void UPlayerHUD::HideText()
{
	if (TextBox)
	{
		ClearText();
		TextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerHUD::SetStamina(float Current, float Max)
{
	if (StaminaGaugeWidget && Max > 0.f)
	{
		ShowStaminaGauge();

		float Ratio = Current / Max;
		StaminaGaugeWidget->UpdateGauge(Ratio);

		if (Max == Ratio) // 게이지가 꽉차면 게이지 숨기기
		{
			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().ClearTimer(StaminaTimerHandle);
				World->GetTimerManager().SetTimer(
					StaminaTimerHandle,
					this,
					&UPlayerHUD::HideStaminaGauge,
					1.5f,
					false);

				FString DebugMessage = FString::Printf(TEXT("Stamina: %.2f"), Current);
				GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::White,DebugMessage);
			}
		}
	}
}

void UPlayerHUD::ShowStaminaGauge()
{
	if (StaminaGaugeWidget)
	{
		StaminaGaugeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UPlayerHUD::HideStaminaGauge()
{
	if (StaminaGaugeWidget)
	{
		StaminaGaugeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
