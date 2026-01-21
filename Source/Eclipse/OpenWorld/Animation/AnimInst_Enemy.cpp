// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Animation/AnimInst_Enemy.h"
#include "OpenWorld/Character/EnemyCharacter.h"

UAnimInst_Enemy::UAnimInst_Enemy()
{
}

void UAnimInst_Enemy::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(BaseOwner);
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimInst_Enemy : Failed Cast BaseOwner to AEnemyCharacter"));
	}
}

void UAnimInst_Enemy::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}
