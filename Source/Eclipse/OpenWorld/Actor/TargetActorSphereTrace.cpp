// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Actor/TargetActorSphereTrace.h"
#include "Abilities/GameplayAbility.h"
#include "OpenWorld/GAS/GA/GameplayAbility_MeleeHitCheck.h"

ATargetActorSphereTrace::ATargetActorSphereTrace()
{
	ShouldProduceTargetDataOnServer = true;
}

void ATargetActorSphereTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	UGameplayAbility_MeleeHitCheck* GA_HitCheck = Cast<UGameplayAbility_MeleeHitCheck>(Ability);

	TraceStartBone = GA_HitCheck->BoneNameA;

	TraceEndBone = GA_HitCheck->BoneNameB;

}


void ATargetActorSphereTrace::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());

	if (IsConfirmTargetingAllowed())
	{
		// BP로 Trace 구현 
		FHitResult HitResult = GetSphereTraceResult(SourceActor, TraceStartBone);

		FGameplayAbilityTargetDataHandle DataHandle(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));

		// Ability Task에 전달
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

