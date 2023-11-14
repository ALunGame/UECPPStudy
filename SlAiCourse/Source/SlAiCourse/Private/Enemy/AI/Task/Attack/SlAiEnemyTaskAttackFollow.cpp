// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Attack/SlAiEnemyTaskAttackFollow.h"

#include "NavigationSystem.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskAttackFollow::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp)) return EBTNodeResult::Failed;

	const float ChaseRadius = 0.f;
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();

	float EPDistance = SPToSE.Size();

	if (EPDistance > 100.f)
	{
		SPToSE.Normalize();
		const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
		FVector DesLoc(0.f);
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SEController, ChaseOrigin, DesLoc, ChaseRadius);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, DesLoc);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName, SECharacter->GetActorLocation());
	}
	return EBTNodeResult::Succeeded;
}
