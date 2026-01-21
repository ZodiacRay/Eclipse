// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatBar.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API UStatBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UStatBar(const FObjectInitializer& ObjectInitializer);

	float GetRatio(); 

protected:

	UPROPERTY()
	TObjectPtr<class UProgressBar> ProgressBar;


	UPROPERTY(BlueprintReadWrite)
	float CurrentStat;

	UPROPERTY(BlueprintReadWrite)
	float MaxStat;



};
