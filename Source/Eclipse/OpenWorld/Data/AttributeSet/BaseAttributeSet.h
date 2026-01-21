// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

UCLASS()
class ECLIPSE_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBaseAttributeSet();

public:
	// 매크로 설정 (Getter Setter 생성) <- 5.6 이후 부터 가능 이전 버전은 복사해야함 매크로 
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, Health);

	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributeSet, Stamina);

	// Attribute 변경될때 호출 
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldValue)
	}

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldValue)
	}


	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina, OldValue)
	}

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina, OldValue)
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// HP
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	// Max HP
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	// Stamina 
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;

	// Max Stamina
	UPROPERTY(BlueprintReadOnly, Category = "Attribute | Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;


};
