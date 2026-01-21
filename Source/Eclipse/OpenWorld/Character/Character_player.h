// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenWorld/Character/BaseCharacter.h"
#include <GameplayEffectTypes.h>
#include "OpenWorld/Interface/Interaction.h"

#include "Character_player.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UGASComponent;
class UPlayerHUD;
class UWidgetComponent; 

UCLASS()
class ECLIPSE_API ACharacter_player : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ACharacter_player(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(const float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void NotifyControllerChanged() override;

#pragma region Components
private:
	// CameraBoom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	// Follow Camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | Throwable", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* Throwable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components | Throwable", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ThrowableMesh;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() { return FollowCamera; }


#pragma endregion

#pragma region Throwing  
protected:
	UPROPERTY(EditAnywhere, Category = "Throwing | Animations")
	UAnimMontage* ThrowingMontage;

#pragma endregion

#pragma region UI Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY()
	UPlayerHUD* HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWidgetComponent* WidgetComponent; 

public:
	void ShowWidgetText(const FText& Message, const FLinearColor& Color, float AutoHideTime = 1.5f);

protected:
#pragma endregion 

#pragma region Input Section 
private:
	// Input Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	UInputMappingContext* DefaultMappingContext;

	// Input Actions 

	// Movement Actions 
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Look Actions 
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* UnlockOnAction;


	// Attack Actions
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ComboAttackAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChargedAttackAction;

	// Interact Actions 
	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:

	// Movement Action Functions 
	void DoMove(const FInputActionValue& Value);

	void DoMove_Walking(const FInputActionValue& Value);

	// Look Action Functions 
	void DoLook(const FInputActionValue& Value);

	void DoZoom(const FInputActionValue& Value);

	// Attack Action Functions


	// Interact Action Functinos 


#pragma endregion

#pragma region Camera Section
protected:

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm"))
	float DefaultCameraDistance = 480.f;

	FVector DefaultCameraPosition = FVector(367.f, 21.15f, 80.f);

	// Zoom In Out Values 
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (ClampMin = 20, ClampMax = 100, Units = "cm"))
	float ZoomStep = 20.f;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (ClampMin = 100, ClampMax = 120, Units = "cm"))
	float MinZoom = 100.f;

	UPROPERTY(EditAnywhere, Category = "Camera", meta = (ClampMin = 120, ClampMax = 1000, Units = "cm"))
	float MaxZoom = 1200.f;

#pragma endregion

#pragma region Movement Values
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Walk")
	float WalkSpeed = 450.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Walk")
	float WalkDirection = -1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement | Sprint")
	bool DisableSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Sprint")
	float SprintRate = 1.7f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Sprint")
	float SprintCost = 0.3f;

public:
	FORCEINLINE float GetWalkDirection() { return WalkDirection; }


#pragma endregion






};
