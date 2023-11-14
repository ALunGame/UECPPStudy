// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationSystem.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Task/SlAiEnemyTaskLookSP.h"

EBTNodeResult::Type USlAiEnemyTaskLookSP::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	const float ChaseRadius = 20.f;
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
	SPToSE.Normalize();

	//起点
	const FVector ChaseOrigin = SEController->GetPlayerLocation() - 100.f * SPToSE;
	FVector DesLoc(0.f);
	//使用导航获得随机点
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SECharacter, ChaseOrigin, DesLoc, ChaseRadius);
	//修改目的地
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName,DesLoc);
	//修改速度
	SECharacter->SetMaxSpeed(300.f);

	
	return EBTNodeResult::Succeeded;
}
