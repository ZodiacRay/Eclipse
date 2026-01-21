// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CustomCharacterMovementComponent.generated.h"


UENUM(BlueprintType)
namespace ECustomMovementMode
{
	enum Type
	{
		MOVE_Climb UMETA(DisplayName = "Climb Mode")
	};
}

UENUM(BlueprintType)
enum ETraceDebugType :uint8
{
	None,
	ForOneFrame,
	ForDuration,
	Persistent
};

UCLASS()
class ECLIPSE_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Owner Character | Animation")
	UAnimInstance* CharacterOwner_AnimInstance;

#pragma region Walk/Sprint Values 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Walk")
	float NormalWalklSpeed = MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	bool DisableSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	float SprintSpeed = MaxWalkSpeed * 1.8;

	UPROPERTY()
	bool bIsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	bool bToggleSprinting; // false -> keydown | true -> Toggle   

#pragma endregion

#pragma region Climbing
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Climbing ")
	bool bIsClimbing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Climbing ")
	float MaxClimbSpeed = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Climbing ")
	float MaxClimbAcceleration = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Climbing ")
	float MaxClimbBreakDeceleration = 400.f;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Climbing | Animation ")
	UAnimMontage* ClimbMontage_Idle;

	
protected:
	// Trace Values 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Climbing | Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> ClimbableSufaceTraceTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Climbing | Trace | Debug")
	TEnumAsByte<ETraceDebugType> ClimbTrace_DebugType = ETraceDebugType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Climbing | Trace")
	float ClimbTrace_CapsuleRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Climbing | Trace")
	float ClimbTrace_CapsuleHalfHeight = 72.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement | Climbing | Trace")
	float ClimbTrace_ShouldStopDegree = 60.f;

private:
	// Trace Results

	TArray<FHitResult> ClimbTrace_SurfaceResults;

	FVector CurrentClimbableSurface_Location;

	FVector CurrentClimbableSurface_Normal;


private:
	TArray<FHitResult> DoCapsuleTraceMultiByObject(const FVector& Start, const FVector& End);

#pragma endregion

#pragma region Dive Values
public:
	UPROPERTY(VisibleAnywhere, Category = "Movement | Dive")
	bool bIsDiving;

	UPROPERTY(EditAnywhere, Category = "Movement | Dive")
	float DiveSpeed = -600.f;

	UPROPERTY(EditAnywhere, Category = "Movement | Dive")
	float DiveGravityScale = 2.5f;

#pragma endregion

#pragma region Override Functions
public:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual float GetMaxSpeed() const override;

	virtual float GetMaxAcceleration() const override;

#pragma endregion

#pragma region Walk/Sprint Functions 
public:

	void StartSprinting();

	void StopSprinting(); 

#pragma endregion 

#pragma region Climbing Functions
public:
	void PhysClimb(float DeltaTime, int32 Iterations);

	// MovementMode 변경 
	void StartClimbing(); 
	void StopClimbing();

	/** 클라이밍을 시작할 수 있는지 */
	bool CanStartClimbing();

	/** 클라이밍 토글 */
	void ToggleClimbing(bool bEnable);

	/** 클라이밍 몽타주 실행*/
	void PlayClimbingMontage(UAnimMontage* MontageToPlay);

	void SnampMovemntToClimbableSurface(float DeltaTime);

	UFUNCTION()
	void OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	// Trace Functions 

	/** 캡슐 트레이스 */
	
	bool FindClimbableSurface();

	/** 라인 트레이스 */
	FHitResult DoLineTraceSingleByObject(const FVector& Start, const FVector& End);
	FHitResult TraceFromEyeHight(float TraceDistance, float TraceStartOffset = 0.f);

	/** 트레이싱 이후 벽 정보*/
	void ProcessClimbableSurfaceInfo(); // 주석 달아야함. 

	/** 클라이밍이 끝나야 하는 경우 */
	//	끝까지 올라간 경우
	bool CheckShouldStopClimbing();

	FQuat GetClimbingRotation(float DeltaTime) const;

public:
	// Getters 
	FVector GetClimbableSurfaceNormal() { return CurrentClimbableSurface_Normal; }

	FVector GetUnrotatedClimbVelocity() const;


#pragma endregion 

#pragma region DiveFunction
	void StartDive();

	void StopDive();

#pragma endregion

};
