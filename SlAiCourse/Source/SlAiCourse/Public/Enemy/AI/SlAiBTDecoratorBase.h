// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SlAiBTDecoratorBase.generated.h"

/**
 * 判断器
 */
UCLASS()
class SLAICOURSE_API USlAiBTDecoratorBase : public UBTDecorator
{
	GENERATED_BODY()

	//检测可以不可以运行
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
