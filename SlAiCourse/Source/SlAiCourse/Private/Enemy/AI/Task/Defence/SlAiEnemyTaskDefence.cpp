// Fill out your copyright notice in the Description page of Project Settings.


#include "Defence/SlAiEnemyTaskDefence.h"

#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyTaskAttackPursuit.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskDefence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	SECharacter->StartDefence();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsDefenceFinish.SelectedKeyName,false);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerPawn.SelectedKeyName,SEController->GetPlayerPawn());

	//计时器
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&USlAiEnemyTaskDefence::OnAnimationTimerDone);
	SEController->GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,2.f,false);
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type USlAiEnemyTaskDefence::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp) || !TimerHandle.IsValid())
	{
		return EBTNodeResult::Aborted;
	}
	SEController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return EBTNodeResult::Aborted;
}

void USlAiEnemyTaskDefence::OnAnimationTimerDone()
{
	if (SEController)
	{
		SEController->FinishStateDefence();
	}
	
}
