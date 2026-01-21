// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Character/AnimInst_Base.h"
#include "OpenWorld/Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UAnimInst_Base::UAnimInst_Base()
{
}

void UAnimInst_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BaseOwner = Cast<ACharacter>(GetOwningActor());

	if (BaseOwner)
	{
		MoveComp = Cast<UCharacterMovementComponent>(BaseOwner->GetCharacterMovement());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAnimInst_Base : Failed Cast to ABaseCharacter. ")); 
	}
}

void UAnimInst_Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (BaseOwner)
	{
		// Update Movement Data 
		UpdateShouldMove();

		UpdateVelocity();

		UpdateDirection();

		UpdateWalkSpeed();

		UpdateAirSpeed();

		UpdateIsFalling();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimInst_Base : Can't find BaseOwner"));
	}

}


void UAnimInst_Base::UpdateVelocity()
{
	Velocity = MoveComp->Velocity;
}

void UAnimInst_Base::UpdateWalkSpeed()
{
	WalkSpeed = UKismetMathLibrary::VSizeXY(BaseOwner->GetVelocity());
}

void UAnimInst_Base::UpdateDirection()
{
	if (!BaseOwner) return;

	FRotator OwnerRoation = BaseOwner->GetActorRotation();
	
	
	Direction = CalculateDirection(Velocity, OwnerRoation);
}

void UAnimInst_Base::UpdateAirSpeed()
{
	AirSpeed = MoveComp->Velocity.Z;
}

void UAnimInst_Base::UpdateShouldMove()
{
	bShouldMove = 
		(MoveComp->GetCurrentAcceleration() != FVector::ZeroVector) &&
		(WalkSpeed > 5.f) && 
		(!bIsFalling);
}

void UAnimInst_Base::UpdateIsFalling()
{
	bIsFalling = MoveComp->IsFalling();
}

float UAnimInst_Base::CalculateDirection(const FVector& InVelocity, const FRotator BaseRotation)
{
	if (!InVelocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = InVelocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = static_cast<float>(FVector::DotProduct(ForwardVector, NormalizedVel));
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		const float RightCosAngle = static_cast<float>(FVector::DotProduct(RightVector, NormalizedVel));
		if (RightCosAngle < 0.f)
		{
			ForwardDeltaDegree *= -1.f;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}
