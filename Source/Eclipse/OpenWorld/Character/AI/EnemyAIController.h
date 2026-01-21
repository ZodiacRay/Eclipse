// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ECLIPSE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController(); 


protected:

	/** 비헤이비어 트리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Behavior Tree")
	UBehaviorTree* BehaviorTreeAsset;

	/** AI 인지 컴포넌트*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI | Perception")
	UAIPerceptionComponent* AIPerception; 


	FTimerHandle TimerHandle; 


	TObjectPtr<ACharacter> ControlledCharacter; 

protected:

	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void OnUnPossess() override;


	void UpdateTarget() const;

	void SetTarget(AActor* NewTarget) const; 

public:

	void StopUpdateTarget();

};
