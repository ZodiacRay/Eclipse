// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenWorld/Character/AnimInst_Base.h"
#include "AnimInst_Player.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API UAnimInst_Player : public UAnimInst_Base
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reference")
	class ACharacter_player* PlayerOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn")
	bool bIsTargetLockOn = false;


public:
	UAnimInst_Player();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
