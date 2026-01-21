// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OpenWorld/Character/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class ATargetPoint;
UCLASS()
class ECLIPSE_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	~AEnemyCharacter();

#pragma region Patrol 
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Patrol")
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

public:
	FORCEINLINE ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}

	FORCEINLINE void IncreasePatrolIndex()
	{
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	}
#pragma endregion
};
