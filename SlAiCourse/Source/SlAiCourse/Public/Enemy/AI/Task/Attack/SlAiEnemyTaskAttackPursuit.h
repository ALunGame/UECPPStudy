// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiEnemyTaskBase.h"
#include "SlAiEnemyTaskAttackPursuit.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USlAiEnemyTaskAttackPursuit : public USlAiEnemyTaskBase
{
	GENERATED_BODY()

	//执行任务
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//终止任务
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	//动作结束的事件
	void OnAnimationTimerDone();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector IsActionFinish;

	FTimerHandle TimerHandle;
};
