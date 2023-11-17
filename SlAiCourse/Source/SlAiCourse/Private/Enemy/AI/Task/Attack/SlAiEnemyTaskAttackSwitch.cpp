// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/Attack/SlAiEnemyTaskAttackSwitch.h"

#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"
#include "SlAiTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USlAiEnemyTaskAttackSwitch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp))
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(PlayerPawn.SelectedKeyName,SEController->GetPlayerPawn());
	if (SEController->IsPlayerDead())
	{
		SEController->LoosePlayer();
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName,(uint8)EEnemyAIState::ES_Patrol);
		return EBTNodeResult::Failed;
	}

	float EPDistance = FVector::Distance(SECharacter->GetActorLocation(), SEController->GetPlayerLocation());
	if (EPDistance < 200.f)
	{
		//普攻
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName,(uint8)EEnemyAttackType::EA_Normal);
		return EBTNodeResult::Succeeded;
	}

	//小于300并且在远离
	if (EPDistance < 300.f && SEController->IsPlayerAway())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName,(uint8)EEnemyAttackType::EA_Pursuit);
		return EBTNodeResult::Succeeded;
	}

	//大于1500.f
	if (EPDistance > 200.f && EPDistance > 300.f )
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AttackType.SelectedKeyName,(uint8)EEnemyAttackType::EA_Dash);
		return EBTNodeResult::Succeeded;
	}
	
	//大于300.f
	if (EPDistance > 300.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(EnemyState.SelectedKeyName,(uint8)EEnemyAIState::ES_Chase);
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
