// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SlAiEnemyTaskWander.h"

#include "NavigationSystem.h"
#include "SlAiEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskWander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	//最大范围
	const float MaxRange = 1000.0f;
	//起点
	const FVector WanderOrigin = SECharacter->GetActorLocation();
	//保存随机位置
	FVector DesLoc(0.f);
	//使用导航获得随机点
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SECharacter, WanderOrigin, DesLoc, MaxRange);
	//当距离小于500，重新找点
	while (FVector::Distance(WanderOrigin, DesLoc) < 500.f)
	{
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SECharacter, WanderOrigin, DesLoc, MaxRange);
	}
	//修改速度
	SECharacter->SetMaxSpeed(100.f);

	//修改目的地
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName,DesLoc);
	//获取停顿时长
	float TotalWaitTime = SECharacter->GetIdleWaitTime();
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName,TotalWaitTime);

	return EBTNodeResult::Succeeded;
}
