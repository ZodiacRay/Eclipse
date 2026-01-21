// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Character/AI/MyBTTask_FindPatrolPoint.h"
#include "OpenWorld/Character/EnemyCharacter.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UMyBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn(); 
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (AEnemyCharacter* Character = Cast<AEnemyCharacter>(ControlledPawn))
	{
		
		// BlackBoard에 PatrolPoint의 Location 저장 
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			BlackBoardLocation.SelectedKeyName, // Behavior Tree에서 변경 
			Character->GetPatrolPoint()->GetActorLocation()
		);

		
		// 다음 Patrol Point로 Index 변경
		Character->IncreasePatrolIndex();
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}
