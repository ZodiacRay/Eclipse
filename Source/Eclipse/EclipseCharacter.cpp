// Copyright Epic Games, Inc. All Rights Reserved.

#include "EclipseCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "OpenWorld/Data/AttributeSet/BaseAttributeSet.h"

#include "Eclipse.h"

AEclipseCharacter::AEclipseCharacter()
{
	// capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// GetCapsuleComponent()->SetCollisionProfileName(TEXT("");
		
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// ASC
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GameplayAbilites"));

	// Attribute Set
	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("CharacterAttributes"));
	ASC->AddAttributeSetSubobject<UBaseAttributeSet>(BaseAttributeSet);
	
}

void AEclipseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ASC 초기화 
	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}
}

void AEclipseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Give Abilities Initial Abilitiese  
	for (const auto& EachAbility : InitialAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(EachAbility));
	}

}

void AEclipseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Blueprint에는 이 함수에 대응하는 이벤트가 없음. 

	// 블루프린트에서 구현 
	EclipseCharacterComponentsInitialize();

}

void AEclipseCharacter::OnOutOfHealthCpp(AActor* InInstigator)
{
	
}


void AEclipseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEclipseCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AEclipseCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEclipseCharacter::Look);

		// Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AEclipseCharacter::DoZoom);

	}
	else
	{
		UE_LOG(LogEclipse, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEclipseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AEclipseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AEclipseCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AEclipseCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AEclipseCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AEclipseCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AEclipseCharacter::DoZoom(const FInputActionValue& Value)
{
	CameraBoom->TargetArmLength = FMath::Clamp(
		CameraBoom->TargetArmLength + Value.GetMagnitude()*ZoomStep, 
		MaxZoomInDistance, 
		MaxZoomOutDistance
	);

}
