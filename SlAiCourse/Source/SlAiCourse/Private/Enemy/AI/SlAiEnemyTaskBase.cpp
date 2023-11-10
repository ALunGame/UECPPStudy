// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\Public\Enemy\AI\SlAiEnemyTaskBase.h"

#include "SlAiEnemyCharacter.h"
#include "SlAiEnemyController.h"

bool USlAiEnemyTaskBase::InitEnemy(UBehaviorTreeComponent& OwnerComp)
{
	if (SECharacter && SEController)
	{
		return true;
	}

	SEController = Cast<ASlAiEnemyController>(OwnerComp.GetAIOwner());
	if (SEController)
	{
		SECharacter = Cast<ASlAiEnemyCharacter>(SEController->GetPawn());
		if (SECharacter)
		{
			return true;
		}
	}

	return false;
}
