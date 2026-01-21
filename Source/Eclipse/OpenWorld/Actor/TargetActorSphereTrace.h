// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "TargetActorSphereTrace.generated.h"




UCLASS()
class ECLIPSE_API ATargetActorSphereTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	ATargetActorSphereTrace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;
	                   

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Targeting")
	FName TraceStartBone;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Targeting")
	FName TraceEndBone;


protected:


	UFUNCTION(BlueprintImplementableEvent)
	FHitResult GetSphereTraceResult(AActor* InSourceActor, FName Bone); // Blueprint에서 구현
};
