// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Character/NonePlayerCharacter.h"
#include "AbilitySystemComponent.h"

ANonePlayerCharacter::ANonePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ANonePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Owner / Avater
	ASC->InitAbilityActorInfo(this, this);
}
