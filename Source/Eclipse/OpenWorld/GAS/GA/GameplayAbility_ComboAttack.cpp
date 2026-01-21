// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/GAS/GA/GameplayAbility_ComboAttack.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameFramework/Character.h"

UGameplayAbility_ComboAttack::UGameplayAbility_ComboAttack()
{
	// 인스턴싱 정책 (능력당 하나의 인스턴스)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//어빌리티 설명 태그 
	const FGameplayTag ComboAttackTag = FGameplayTag::RequestGameplayTag(FName("Character.Ability.Attack"));
	AbilityTags.AddTag(ComboAttackTag);



	// 트리거 이벤트 태그 설정 
	const FGameplayTag AttackStartTag = FGameplayTag::RequestGameplayTag(FName("Event.Character.Attack.Start"));

		// 트리거 데이터 
	FAbilityTriggerData Trigger;
	Trigger.TriggerTag = AttackStartTag;
	Trigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		// 트리거로 설정 
	AbilityTriggers.Add(Trigger);



	// GA 실행 중에 ASC에 줄 태그 
	const FGameplayTag AttackingTag = FGameplayTag::RequestGameplayTag("Character.State.Attacking");
	ActivationOwnedTags.AddTag(AttackingTag);

}

bool UGameplayAbility_ComboAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return true;
}

void UGameplayAbility_ComboAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
	)
{
	// 능력이 사용 가능 한지 체크 ( 코스트 / 쿨타임 등) 
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// ASC를 가져옴 
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	

}
