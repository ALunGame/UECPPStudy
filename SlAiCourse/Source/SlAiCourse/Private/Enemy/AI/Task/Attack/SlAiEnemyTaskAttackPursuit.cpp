// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Attack/SlAiEnemyTaskAttackPursuit.h"

#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "SlAiTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskAttackPursuit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	//播放冲刺动画
	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Pursuit);
	SECharacter->SetMaxSpeed(600.f);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsActionFinish.SelectedKeyName,false);

	//计时器
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone);
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,AttackDuration,false);
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskAttackPursuit::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp) || !TimerHandle.IsValid())
	{
		return EBTNodeResult::Aborted;
	}
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskAttackPursuit::OnAnimationTimerDone()
{
	if (SEController)
	{
		SEController->ResetProcess(true);
	}
	if (SECharacter)
	{
		SECharacter->SetMaxSpeed(300.f);
	}
}
