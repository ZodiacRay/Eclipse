#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_UpdateMoveSpeed.generated.h"

UCLASS()
class ECLIPSE_API UBTTaskNode_UpdateMoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
	// AI Character의 걷기 속도를 바꾸는 Task 

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed = 100.f;

};
