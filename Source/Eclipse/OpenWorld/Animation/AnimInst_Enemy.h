// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenWorld/Character/AnimInst_Base.h"
#include "OpenWorld/Character/BaseCharacter.h"
#include "OpenWorld/Character/EnemyCharacter.h"

#include "AnimInst_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API UAnimInst_Enemy : public UAnimInst_Base
{
	GENERATED_BODY()
	
public:
	UAnimInst_Enemy();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
