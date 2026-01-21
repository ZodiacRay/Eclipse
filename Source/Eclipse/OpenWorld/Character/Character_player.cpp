// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Character/Character_player.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include"Components/CapsuleComponent.h"
#include "OpenWorld/UI/PlayerHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OpenWorld/Actor/ThrowableActor.h"
#include "OpenWorld/Interface/Interactable.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



 
ACharacter_player::ACharacter_player(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	// Create Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultCameraDistance;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	// Create a Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->SetRelativeLocation(DefaultCameraPosition);

	ThrowableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ThrowableMesh"));
	ThrowableMesh->SetupAttachment(ACharacter::GetMesh());
	ThrowableMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_r"));

}

#pragma region Override Function

void ACharacter_player::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: Character %s"), *GetName());

	if (PlayerHUDWidgetClass)
	{
		HUDWidget = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDWidgetClass);

		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}

	}
}

void ACharacter_player::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ACharacter_player::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// ASC->InitAbilityActorInfo(this, this); // ASC's Owner, Avater is 

}

void ACharacter_player::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

#pragma endregion


#pragma region Input Functions 

void ACharacter_player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement Actions Bind 
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacter_player::DoMove);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter_player::Jump);

		// Look Actions Bind 
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacter_player::DoLook);

		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ACharacter_player::DoZoom);


	}
}

// Movement Action Functions 
void ACharacter_player::DoMove(const FInputActionValue& Value)
{
	DoMove_Walking(Value);

}

void ACharacter_player::DoMove_Walking(const FInputActionValue& Value)
{
	if (!Controller) return;

	// Input is Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	float ForwardValue = MovementVector.Y;
	float RightValue = MovementVector.X;

	// Find Forward 
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get Forward Right Vector
	const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	// Add Movement 
	AddMovementInput(ForwardDir, ForwardValue);
	AddMovementInput(RightDir, RightValue);
}


// Look Action Functions
void ACharacter_player::DoLook(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D LookDir = Value.Get<FVector2D>();
	
	AddControllerPitchInput(LookDir.Y);

	AddControllerYawInput(LookDir.X);
}

void ACharacter_player::DoZoom(const FInputActionValue& Value)
{
	const float AxisValue = Value.Get<float>();

	CameraBoom->TargetArmLength = FMath::Clamp(
		CameraBoom->TargetArmLength - AxisValue * ZoomStep,
		MinZoom, MaxZoom
	);
}


#pragma endregion


#pragma region UI Functions

void ACharacter_player::ShowWidgetText(const FText& Message, const FLinearColor& Color, float AutoHideTime)
{
	HUDWidget->ShowText(Message, Color, AutoHideTime);
}

#pragma endregion 



