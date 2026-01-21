// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "BaseCharacter.generated.h"


UCLASS()
class ECLIPSE_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

#pragma region Components
protected:
	
	// Custom Movement Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UCustomCharacterMovementComponent* CustomMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// Ability System Compoent 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;
#pragma endregion

#pragma region GAS 
protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	EGameplayEffectReplicationMode ASCReplicationMode = EGameplayEffectReplicationMode::Mixed;*/

	//  Abilities Character has  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	/** Attributes (Health, Stamina) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | Attribute")
	TObjectPtr<class UBaseAttributeSet> BaseAttributeSet;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<struct FGameplayAbilitySpecHandle> InitailAbilitySpecHandles;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }

	

	// 블루프린트에서 구현 
	UFUNCTION(BlueprintImplementableEvent)
	void InitialzieAbilitySystemComponent();


#pragma endregion

#pragma region Override Functinos 
public:

	virtual void PossessedBy(AController* NewController) override;

	// 플레이어가 복제되는  서버 서버에 들어오거나 스폰됨 
	virtual void OnRep_PlayerState() override;

	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma endregion

#pragma region Getter


	 UCustomCharacterMovementComponent* GetCustomMovement() { return CustomMovementComponent;}
#pragma endregion
};
