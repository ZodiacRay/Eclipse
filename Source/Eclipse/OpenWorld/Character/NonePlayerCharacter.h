// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenWorld/Character/BaseCharacter.h"
#include "NonePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API ANonePlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	ANonePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;
};
