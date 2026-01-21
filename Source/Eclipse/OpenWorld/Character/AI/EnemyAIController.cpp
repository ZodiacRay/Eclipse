// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorld/Character/AI/EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledCharacter = Cast<ACharacter>(InPawn);

	// BehaviorTree 실행 
	RunBehaviorTree(BehaviorTreeAsset);

	// UpdateTarget 타이머 등록 
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, 
		this, 
		&AEnemyAIController::UpdateTarget,
		0.1f, // 등록 주기 
		true
	);


}

void AEnemyAIController::OnUnPossess()
{
	StopUpdateTarget(); 

	ControlledCharacter = nullptr;

	Super::OnUnPossess();
}

void AEnemyAIController::UpdateTarget() const
{
	TArray<AActor*> OutActors;
	AIPerception->GetKnownPerceivedActors(nullptr, OutActors);

	ACharacter* Enemy = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (OutActors.Contains(Enemy))
	{
		
		
		SetTarget(Enemy);

		

		//UE_LOG(LogTemp, Warning, TEXT("Find Target")); 
	}
	else
	{
		SetTarget(nullptr);

		//UE_LOG(LogTemp, Warning, TEXT("Fail to Find Target"));
	}

}

void AEnemyAIController::SetTarget(AActor* NewTarget) const
{
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(FName("ChaseTarget"), NewTarget);
	}

	if (IsValid(ControlledCharacter))
	{

	}
}

void AEnemyAIController::StopUpdateTarget()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	SetTarget(nullptr);
}
