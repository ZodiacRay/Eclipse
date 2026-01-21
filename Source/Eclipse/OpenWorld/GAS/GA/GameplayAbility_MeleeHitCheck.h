// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_MeleeHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API UGameplayAbility_MeleeHitCheck : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Targeting")
	FName BoneNameA;

	UPROPERTY(BlueprintReadWrite, Category = "Targeting")
	FName BoneNameB;
};
