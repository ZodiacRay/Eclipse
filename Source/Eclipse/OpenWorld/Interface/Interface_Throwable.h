// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Throwable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Throwable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECLIPSE_API IInterface_Throwable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Throwed(AActor* Interactor) = 0; 
};
