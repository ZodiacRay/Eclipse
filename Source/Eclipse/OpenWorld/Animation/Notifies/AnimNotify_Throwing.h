// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Throwing.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API UAnimNotify_Throwing : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UAnimNotify_Throwing(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
