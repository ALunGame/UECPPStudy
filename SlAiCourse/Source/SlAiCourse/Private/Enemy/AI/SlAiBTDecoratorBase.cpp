﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SlAiBTDecoratorBase.h"

bool USlAiBTDecoratorBase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
