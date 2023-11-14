// Fill out your copyright notice in the Description page of Project Settings.


#include "Task/SlAiEnemyTaskRotate.h"

#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"

EBTNodeResult::Type USlAiEnemyTaskRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!InitEnemy(OwnerComp)) return EBTNodeResult::Failed;

    FVector SEToSP = FVector(SEController->GetPlayerLocation().X,SEController->GetPlayerLocation().Y,0.f) -
    	FVector(SECharacter->GetActorLocation().X,SECharacter->GetActorLocation().Y,0.f);
	SEToSP.Normalize();

	//新朝向
	SECharacter->UpdateRotation(FRotationMatrix::MakeFromX(SEToSP).Rotator());
	
	return EBTNodeResult::Succeeded;

}
