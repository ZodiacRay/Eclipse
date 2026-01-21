 // Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Component/CharacterComponent/CustomCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "OpenWorld/Character/BaseCharacter.h"


UCustomCharacterMovementComponent::UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer) 
{
}

#pragma region Override Functions 
void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterOwner)
	{
		CharacterOwner_AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	}

	if (CharacterOwner_AnimInstance)
	{

	}
}

void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (bIsClimbing)
	{
		// 캐릭터가 이동해도 캐릭터의 방향 고정 
		bOrientRotationToMovement = false;

		// 캐릭터 캡슐 사이즈 조정 
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);

		// TODO: Bind Climb Delegate 
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == ECustomMovementMode::MOVE_Climb)
	{
		// 캐릭터가 이동하는 방향에 맞게 캐릭터의 방향 조정 
		bOrientRotationToMovement = true;

		// 캐릭터 캡슐 사이즈 조정 
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);

		// 캐릭터의 회전값 정리 
		const FRotator DirtyRotation = UpdatedComponent->GetComponentRotation();
		const FRotator CleanRotation = FRotator(0.f, DirtyRotation.Yaw, 0.f);

		UpdatedComponent->SetRelativeRotation(CleanRotation);

		// 모드 변경시 남아있는 관성, 속도 제거 
		StopMovementImmediately();

	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UCustomCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	if (bIsClimbing)
	{
		PhysClimb(DeltaTime, Iterations);
	}


	Super::PhysCustom(DeltaTime, Iterations);
}

float UCustomCharacterMovementComponent::GetMaxSpeed() const
{
	if (bIsClimbing)
	{
		return MaxClimbSpeed;
	}

	return Super::GetMaxSpeed();
}

float UCustomCharacterMovementComponent::GetMaxAcceleration() const
{
	if (bIsClimbing)
	{
		return MaxClimbAcceleration;
	}

	return Super::GetMaxAcceleration();
}
#pragma endregion


#pragma region Walk/Sprint Functions 


void UCustomCharacterMovementComponent::StartSprinting()
{
	if (bIsSprinting) return;

	bIsSprinting = true; 

	MaxWalkSpeed = SprintSpeed;
}

void UCustomCharacterMovementComponent::StopSprinting()
{
	bIsSprinting = false;

	MaxWalkSpeed = NormalWalklSpeed;
}

#pragma endregion

#pragma region Climb Functions

void UCustomCharacterMovementComponent::PhysClimb(float DeltaTime, int32 Iterations)
{
	// DeltaTime이 너무 작으면 함수를 종료해 연산 수를 줄임.
	if (DeltaTime < MIN_TICK_TIME) return;

	// 클라이밍 가능한 벽을 찾고 벽의 정보 처리 
	FindClimbableSurface();
	ProcessClimbableSurfaceInfo();

	if (CheckShouldStopClimbing())
	{
		// 클라이밍 중지 
		ToggleClimbing(false);
	}

	// 애니메이션 루트모션으로 추가된 속도를 제거
	RestorePreAdditiveRootMotionVelocity();

	// 루트모션을 사용하지 않는 경우 
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		// 
		CalcVelocity(DeltaTime, 0.f, true, MaxClimbBreakDeceleration);
	}

	// 애니메이션 루트모션으로 추가된 속도 제거 
	ApplyRootMotionToVelocity(DeltaTime);

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * DeltaTime;
	FHitResult Hit(1.f);

	UpdatedComponent->MoveComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		HandleImpact(Hit, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, 1.f - Hit.Time, Hit.Normal, Hit, true);
	}

	/*if (Hit.Time < 1.f)
	{

		HandleImpact(Hit, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, 1.f - Hit.Time, Hit.Normal, Hit, true);
	}*/

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
	}

	// Snap movement to climbable surface
	SnampMovemntToClimbableSurface(DeltaTime);
}

void UCustomCharacterMovementComponent::StartClimbing()
{
	SetMovementMode(MOVE_Custom, ECustomMovementMode::MOVE_Climb);
}

void UCustomCharacterMovementComponent::StopClimbing()
{
	SetMovementMode(MOVE_Falling);
}

bool UCustomCharacterMovementComponent::CanStartClimbing()
{
	// 벽이 없는 경우 
	if (!FindClimbableSurface())
	{
		return false;
	}

	// 벽이 눈 밑에 있는 경우 
	if (!TraceFromEyeHight(100.f).bBlockingHit)
	{
		return false;
	}

	return true;
}

void UCustomCharacterMovementComponent::ToggleClimbing(bool bEnable)
{
	if (bEnable)
	{
		if (CanStartClimbing())
		{
			// Movement Mode 변경 
			StartClimbing();

			// Montage 실행
			PlayClimbingMontage(ClimbMontage_Idle);
		}
	}
	else
	{
		// Movement Mode 변경
		StopClimbing();
	}
}

void UCustomCharacterMovementComponent::PlayClimbingMontage(UAnimMontage* MontageToPlay)
{
	if (
		!MontageToPlay || // 몽타주가 null
		!CharacterOwner_AnimInstance || // 애님인스턴스가 null
		!CharacterOwner_AnimInstance->IsAnyMontagePlaying() //  몽타주가 실행중인 경우
		)
	{
		return;
	}
	
	// 몽타주 실행 
	CharacterOwner_AnimInstance->Montage_Play(MontageToPlay);

}

void UCustomCharacterMovementComponent::SnampMovemntToClimbableSurface(float DeltaTime)
{
	// 캐릭터의 정면 
	const FVector ComponentForward = UpdatedComponent->GetForwardVector();
	// 캐릭터 위치 
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();

	const FVector ProjectedCharacterToSurface =
		(CurrentClimbableSurface_Location - ComponentLocation).ProjectOnTo(ComponentForward);

	const FVector SnapVector = -CurrentClimbableSurface_Normal * ProjectedCharacterToSurface.Length();

	UpdatedComponent->MoveComponent(
		SnapVector * DeltaTime * MaxClimbSpeed,
		UpdatedComponent->GetComponentQuat(),
		true
	);
}

void UCustomCharacterMovementComponent::OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == ClimbMontage_Idle)
	{
		StartClimbing();
	}
}

TArray<FHitResult> UCustomCharacterMovementComponent::DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End)
{
	TArray<FHitResult> OutTraceResult;

	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	switch (ClimbTrace_DebugType)
	{
	case ETraceDebugType::ForDuration:
		DebugType = EDrawDebugTrace::ForDuration;
		break;

	case ETraceDebugType::ForOneFrame:
		DebugType = EDrawDebugTrace::ForOneFrame;
		break;

	case ETraceDebugType::Persistent:
		DebugType = EDrawDebugTrace::Persistent;
		break;
	}


	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this,
		Start,
		End,
		ClimbTrace_CapsuleRadius,		// CapsuleTrace Radius
		ClimbTrace_CapsuleHalfHeight,	// CapsuleTrace Height's 1/2 
		ClimbableSufaceTraceTypes,		// Object Types
		false,							// bTraceComplex
		TArray<AActor*>(),				// Ignore Actors
		DebugType,					// Draw Debug or Not
		OutTraceResult,
		false // bIgnoreSelf
	);

	return OutTraceResult;

}

bool UCustomCharacterMovementComponent::FindClimbableSurface()
{
	const FVector StartOffset = UpdatedComponent->GetForwardVector() * 30.f;
	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector();

	ClimbTrace_SurfaceResults = DoCapsuleTraceMultiByObject(Start, End);

	return !ClimbTrace_SurfaceResults.IsEmpty();
}

FHitResult UCustomCharacterMovementComponent::DoLineTraceSingleByObject(const FVector& Start, const FVector& End)
{
	FHitResult OutHit;

	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	switch (ClimbTrace_DebugType)
	{
	case ETraceDebugType::ForDuration:
		DebugType = EDrawDebugTrace::ForDuration;
		break;

	case ETraceDebugType::ForOneFrame:
		DebugType = EDrawDebugTrace::ForOneFrame;
		break;

	case ETraceDebugType::Persistent:
		DebugType = EDrawDebugTrace::Persistent;
		break;
	}


	// 라인 트레이스 
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ClimbableSufaceTraceTypes,
		false,
		TArray<AActor*>(),	// ignore Actors 
		DebugType,		// Debug setting 
		OutHit,				// result 
		false				// ignore self 
	);

	return OutHit;
}

FHitResult UCustomCharacterMovementComponent::TraceFromEyeHight(float TraceDistance, float TraceStartOffset)
{
	const FVector ComponentLocation = UpdatedComponent->GetComponentLocation();
	const FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + TraceStartOffset);

	const FVector Start = ComponentLocation + EyeHeightOffset;
	const FVector End = Start + (CharacterOwner->GetActorForwardVector() * TraceDistance);

	return DoLineTraceSingleByObject(Start, End);
}

void UCustomCharacterMovementComponent::ProcessClimbableSurfaceInfo()
{
	// 위치 노멀 초기화 
	CurrentClimbableSurface_Location = FVector::ZeroVector;
	CurrentClimbableSurface_Normal = FVector::ZeroVector;

	// 감지된 벽이 없는 경우 
	if (ClimbTrace_SurfaceResults.IsEmpty()) return;


	for (const FHitResult& TraceHitResult : ClimbTrace_SurfaceResults)
	{
		CurrentClimbableSurface_Location += TraceHitResult.ImpactPoint;
		CurrentClimbableSurface_Normal += TraceHitResult.ImpactNormal;
	}

	CurrentClimbableSurface_Location /= ClimbTrace_SurfaceResults.Num();
	CurrentClimbableSurface_Normal = CurrentClimbableSurface_Normal.GetSafeNormal();
}

bool UCustomCharacterMovementComponent::CheckShouldStopClimbing()
{
	// 트레이스한 결과가 없으면 중단 
	if (ClimbTrace_SurfaceResults.IsEmpty()) return true;

	// 벽의 노멀과 업벡터를 내적해서 각도 구하기 (라디안)
	const float DotResult = FVector::DotProduct(
		CurrentClimbableSurface_Normal, FVector::UpVector
	);

	// 라디안을 각도로 변환 
	const float DegreeDiff = FMath::RadiansToDegrees(FMath::Acos(DotResult));

	// 각도가 작은 경우 중단 
	if (DegreeDiff <= ClimbTrace_ShouldStopDegree) return true;

	return false;
}

FQuat UCustomCharacterMovementComponent::GetClimbingRotation(float DeltaTime) const
{
	const FQuat CurrentQuat = UpdatedComponent->GetComponentQuat();

	if (HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity())
	{
		return CurrentQuat;
	}

	const FQuat TargetQuat = FRotationMatrix::MakeFromX(-CurrentClimbableSurface_Normal).ToQuat();
	return FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.f);
}

FVector UCustomCharacterMovementComponent::GetUnrotatedClimbVelocity() const
{
	return UKismetMathLibrary::Quat_UnrotateVector(UpdatedComponent->GetComponentQuat(), Velocity);
}
#pragma endregion



void UCustomCharacterMovementComponent::StartDive()
{
	bIsDiving = true;

	// 공기저항 감소
	BrakingDecelerationFalling = 0.f;

	// 중력 더 강하게 
	GravityScale = DiveGravityScale; 
}

void UCustomCharacterMovementComponent::StopDive()
{
	bIsDiving = false;

	BrakingDecelerationFalling = 2048.f;
	GravityScale = 1.f;
}



