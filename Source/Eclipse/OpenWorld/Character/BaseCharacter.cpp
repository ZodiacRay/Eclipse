// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "OpenWorld/Component/CharacterComponent/CustomCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "OpenWorld/Data/AttributeSet/BaseAttributeSet.h"



ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Contorller Rotaions 
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Set Size for Collision Capsule 
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Set Movement Values  
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	// Ability System Component 
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GameplayAbilites"));
	
	// Attribute Set 
	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	// Give Default Abilities
	for (const auto& EachAbility : InitialAbilities)
	{
		FGameplayAbilitySpecHandle EachSpecHandle = ASC->GiveAbility(FGameplayAbilitySpec(EachAbility));
		InitailAbilitySpecHandles.Add(EachSpecHandle);
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
	}
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitialzieAbilitySystemComponent();

	// Initialize Attribute Set 
	/*ASC->SetNumericAttributeBase(BaseAttributeSet->GetMaxHealthAttribute(), 100.f);
	ASC->SetNumericAttributeBase(BaseAttributeSet->GetHealthAttribute(), 100.f);

	ASC->SetNumericAttributeBase(BaseAttributeSet->GetMaxStaminaAttribute(), 100.f);
	ASC->SetNumericAttributeBase(BaseAttributeSet->GetStaminaAttribute(), 100.f);*/

}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


