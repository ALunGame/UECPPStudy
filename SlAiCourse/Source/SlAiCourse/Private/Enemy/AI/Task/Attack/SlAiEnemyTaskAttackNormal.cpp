// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Attack/SlAiEnemyTaskAttackNormal.h"

#include "SlAiEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskAttackNormal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	float AttackDuration = SECharacter->PlayAttackAction(EEnemyAttackType::EA_Normal);
	//设置参数
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(WaitTime.SelectedKeyName,AttackDuration);
	
	return EBTNodeResult::Succeeded;
}
