// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Character/AI/Tasks/BTTaskNode_UpdateMoveSpeed.h"

#include "AIController.h"
#include "OpenWorld/Character/BaseCharacter.h"
#include "OpenWorld/Component/CharacterComponent/CustomCharacterMovementComponent.h"


EBTNodeResult::Type UBTTaskNode_UpdateMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseCharacter* ControlledPawn = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControlledPawn) return EBTNodeResult::Failed;

	if (UCustomCharacterMovementComponent* MovementComp = ControlledPawn->GetCustomMovement())
	{
		MovementComp->MaxWalkSpeed = MaxWalkSpeed;

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
