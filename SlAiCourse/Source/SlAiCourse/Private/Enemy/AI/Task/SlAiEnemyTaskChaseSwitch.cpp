// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/SlAiEnemyTaskChaseSwitch.h"
#include "SlAiEnemyController.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiTypes.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskChaseSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
	if (EPDistance < 300.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName,(uint8)EEnemyAIState::ES_Attack);
		return EBTNodeResult::Failed;
	}
	
	//大于1500.f
	if (EPDistance > 1500.f)
	{
		SEController->LoosePlayer();
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName,(uint8)EEnemyAIState::ES_Patrol);
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
