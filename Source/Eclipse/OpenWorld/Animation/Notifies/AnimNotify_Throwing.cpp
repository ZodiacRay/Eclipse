// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Animation/Notifies/AnimNotify_Throwing.h"
#include "OpenWorld/Character/Character_player.h"

UAnimNotify_Throwing::UAnimNotify_Throwing(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotify_Throwing::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

}
