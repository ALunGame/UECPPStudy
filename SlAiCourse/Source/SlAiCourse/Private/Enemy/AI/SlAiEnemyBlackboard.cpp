// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SlAiEnemyBlackboard.h"

#include "SlAiPlayerCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

void USlAiEnemyBlackboard::PostLoad()
{
	Super::PostLoad();

	//目的地
	FBlackboardEntry Destination;
	Destination.EntryName = FName("Destination");
	UBlackboardKeyType_Vector* DestinationKeyType = NewObject<UBlackboardKeyType_Vector>();
	Destination.KeyType = DestinationKeyType;
	Keys.Add(Destination);

	//敌人状态
	FBlackboardEntry EnemyState;
	EnemyState.EntryName = FName("EnemyState");
	UBlackboardKeyType_Enum* EnemyStateKeyType = NewObject<UBlackboardKeyType_Enum>();
	//俩个都可以实现绑定枚举
	EnemyStateKeyType->EnumType = FindObject<UEnum>(ANY_PACKAGE,*FString("EEnemyAIState"),true);
	EnemyStateKeyType->EnumName = FString("EEnemyAIState");
	EnemyState.KeyType = EnemyStateKeyType;
	Keys.Add(EnemyState);

	//等待时间
	FBlackboardEntry WaitTime;
	WaitTime.EntryName = FName("WaitTime");
	WaitTime.KeyType = NewObject<UBlackboardKeyType_Float>();
	Keys.Add(WaitTime);

	//攻击类型
	FBlackboardEntry AttackType;
	AttackType.EntryName = FName("AttackType");
	UBlackboardKeyType_Enum* EnemyAttackKeyType = NewObject<UBlackboardKeyType_Enum>();
	EnemyAttackKeyType->EnumType = FindObject<UEnum>(ANY_PACKAGE,*FString("EEnemyAttackType"),true);
	EnemyAttackKeyType->EnumName = FString("EEnemyAttackType");
	AttackType.KeyType = EnemyAttackKeyType;
	Keys.Add(AttackType);

	//玩家指针
	FBlackboardEntry PlayerPawn;
	PlayerPawn.EntryName = FName("PlayerPawn");
	UBlackboardKeyType_Object* PlayerPawnKeyType = NewObject<UBlackboardKeyType_Object>();
	PlayerPawnKeyType->BaseClass = ASlAiPlayerCharacter::StaticClass();
	PlayerPawn.KeyType = PlayerPawnKeyType;
	Keys.Add(PlayerPawn);

	//动作是否完成
	FBlackboardEntry ProcessFinish;
	ProcessFinish.EntryName = FName("ProcessFinish");
	ProcessFinish.KeyType = NewObject<UBlackboardKeyType_Bool>();
	Keys.Add(ProcessFinish);
}
