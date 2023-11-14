// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Attack/SlAiEnemyTaskAttackDash.h"

#include "NavigationSystem.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "SlAiTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskAttackDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	//播放冲刺动画
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Dash);

	const float ChaseRadius = 0.f;
	FVector SPToSE = SEController->GetPlayerLocation() - SECharacter->GetActorLocation();
	SPToSE.Normalize();

	const FVector ChaseOrigin = SEController->GetPlayerLocation() - 20.f * SPToSE;

	FVector DesLoc(0.f);
	//使用导航获得随机点
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(SECharacter, ChaseOrigin, DesLoc, ChaseRadius);
	//角色速度
	float Speed = (FVector::Distance(SECharacter->GetActorLocation(),DesLoc)) / AttackDuration + 30.f;
	//修改速度
	SECharacter->SetMaxSpeed(Speed);
	
	//修改目的地
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(Destination.SelectedKeyName,DesLoc);
	//设置参数
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName,AttackDuration);

	//计时器
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&USlAiEnemyTaskAttackDash::OnAnimationTimerDone);
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,AttackDuration,false);
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskAttackDash::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp) || !TimerHandle.IsValid())
	{
		return EBTNodeResult::Aborted;
	}
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskAttackDash::OnAnimationTimerDone()
{
	if (SECharacter)
	{
		SECharacter->SetMaxSpeed(300.f);
	}
}
