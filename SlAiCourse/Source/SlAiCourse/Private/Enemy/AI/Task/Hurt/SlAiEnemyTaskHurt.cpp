// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Hurt/SlAiEnemyTaskHurt.h"

#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "SlAiTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskHurt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	//播放受伤动画
	float HurtDuration = SECharacter->PlayHurtAction();

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName,HurtDuration);
	
	//计时器
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&USlAiEnemyTaskHurt::OnAnimationTimerDone);
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,HurtDuration,false);
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskHurt::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp) || !TimerHandle.IsValid())
	{
		return EBTNodeResult::Aborted;
	}
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskHurt::OnAnimationTimerDone()
{
	SEController->FinishStateHurt();
}
