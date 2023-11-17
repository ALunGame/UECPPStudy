// Fill out your copyright notice in the Description page of Project Settings.


#include "Defence/SlAiEnemyTaskLocaESC.h"

#include "NavigationSystem.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskLocaESC::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	const float ChaseRadius = 2000.f;
	FVector SPToSE = SECharacter->GetActorLocation() - SEController->GetPlayerLocation();
	SPToSE.Normalize();

	//起点
	const FVector ChaseOrigin = SECharacter->GetActorLocation();

	FVector DesLoc(0.f);
	//使用导航获得随机点
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SECharacter, ChaseOrigin, DesLoc, ChaseRadius);
	//逃跑方向大于90度
	while (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SPToSE,DesLoc - SECharacter->GetActorLocation()))) > 90.f)
	{
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SECharacter, ChaseOrigin, DesLoc, ChaseRadius);
	}

	//修改目的地
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName,DesLoc);
	//修改速度
	SECharacter->SetMaxSpeed(300.f);
	return EBTNodeResult::Succeeded;
}
