// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_ComboAttack.generated.h"




UCLASS()
class ECLIPSE_API UGameplayAbility_ComboAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_ComboAttack();

	// 현재 상태 태그를 확인해서 어빌리티 활성화 결정 
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		OUT FGameplayTagContainer* OptionalRelevantTags
	)const override;

	// 능력이 활성화 되면 호출 
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool ComboBuffer;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* MontageToPlay;

	
};
