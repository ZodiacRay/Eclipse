// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/UI/StatBar.h"
#include "Components/ProgressBar.h"

UStatBar::UStatBar(const FObjectInitializer& ObjectInitializer) 
	:Super(ObjectInitializer)
{
	ProgressBar = CreateDefaultSubobject<UProgressBar>(TEXT("StatBar"));
}


float UStatBar::GetRatio()
{
	return FMath::Clamp( CurrentStat / MaxStat, 0.f, 1.f); 
}
