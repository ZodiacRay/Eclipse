// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Animation/AnimInst_Player.h"
#include "OpenWorld/Character/Character_player.h"

UAnimInst_Player::UAnimInst_Player()
{

}

void UAnimInst_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (BaseOwner)
	{
		PlayerOwner = Cast<ACharacter_player>(BaseOwner);
	}

}

void UAnimInst_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	

	
}


