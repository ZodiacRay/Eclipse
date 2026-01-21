// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInst_Base.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API UAnimInst_Base : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	class ACharacter* BaseOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	class UCharacterMovementComponent* MoveComp;

protected:

	/** 움직이고 있는지 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bShouldMove;

	/** 떨어지는 중인지 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bIsFalling;
	
	/** 속도 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data")
	FVector Velocity;

	/** Walk Sprint 속력*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data ")
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data | Speed")
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement Data | Speed")
	float AirSpeed;

#pragma region Override Functions 
public:
	UAnimInst_Base();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion

#pragma region Update Movement Data Functions
public:

	void UpdateVelocity();

	void UpdateWalkSpeed();

	void UpdateDirection();

	void UpdateAirSpeed();

	void UpdateShouldMove();

	void UpdateIsFalling();


#pragma endregion

	float CalculateDirection(const FVector& InVelocity, const FRotator BaseRotation);
	
};
