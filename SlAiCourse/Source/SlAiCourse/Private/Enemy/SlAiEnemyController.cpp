// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SlAiEnemyController.h"

#include "SlAiEnemyBlackboard.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiPlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASlAiEnemyController::ASlAiEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

	IsLockPlayer = false;
}

// Called when the game starts or when spawned
void ASlAiEnemyController::BeginPlay()
{
	//必须调用父类
	Super::BeginPlay();

	//保存玩家指针
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GWorld,0));

	if (!SECharacter)
	{
		SECharacter = Cast<ASlAiEnemyCharacter>(GetPawn());
	}

	FTimerDelegate EPDisDele = FTimerDelegate::CreateUObject(this, &ASlAiEnemyController::UpdateStateParam);
	GetWorld()->GetTimerManager().SetTimer(EPDisHandle,EPDisDele,0.3f,true);

	HPRatio = 1;
	IsAllowHurt = false;
	HurtTimeCnt = 0.f;
}

void ASlAiEnemyController::UpdateStateParam()
{
	//更新距离序列
	if (EPDisList.Num() < 6)
	{
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(),GetPlayerLocation()));
	}
	else
	{
		EPDisList.RemoveAt(0);
		EPDisList.Push(FVector::Distance(SECharacter->GetActorLocation(),GetPlayerLocation()));
	}

	//更新受伤序列。受伤后6秒不允许进入受伤
	if (HurtTimeCnt < 6.f)
	{
		HurtTimeCnt += 0.3f;
	}
	else
	{
		HurtTimeCnt = 0.f;
		IsAllowHurt = true;
	}
}

// Called every frame
void ASlAiEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SPCharacter && SECharacter)
	{
		SECharacter->UpdateHPBarRotation(SPCharacter->ThirdCamera->GetComponentLocation());
	}
}

//在BeginPlay之前运行
void ASlAiEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SECharacter = Cast<ASlAiEnemyCharacter>(GetPawn());

	//获得行为树资源
	UBehaviorTree* StaticBehaviorTree = LoadObject<UBehaviorTree>(nullptr,TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));
	//赋值一份行为树
	UBehaviorTree* BehaviorTreeObject = DuplicateObject<UBehaviorTree>(StaticBehaviorTree,nullptr);
	if (!BehaviorTreeObject)
	{
		return;
	}
	BehaviorTreeObject->BlackboardAsset = DuplicateObject<USlAiEnemyBlackboard>((USlAiEnemyBlackboard*)StaticBehaviorTree->BlackboardAsset,nullptr);

	//绑定黑板资源
	BlackboardComp = Blackboard;
	bool IsSuccess = false;
	if (BehaviorTreeObject->BlackboardAsset && (Blackboard == nullptr || Blackboard->IsCompatibleWith(BehaviorTreeObject->BlackboardAsset) == false))
	{
		IsSuccess = UseBlackboard(BehaviorTreeObject->BlackboardAsset,BlackboardComp);
	}

	//运行行为树
	if (IsSuccess)
	{
		BehaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);
		if (!BehaviorComp)
		{
			BehaviorComp = NewObject<UBehaviorTreeComponent>(this,TEXT("BehaviorComp"));
			BehaviorComp->RegisterComponent();
		}
		BrainComponent = BehaviorComp;
		check(BehaviorComp != nullptr);
		BehaviorComp->StartTree(*BehaviorTreeObject, EBTExecutionMode::Looped);
	
		//设置状态为巡逻
		BlackboardComp->SetValueAsEnum("EnemyState",(uint8)EEnemyAIState::ES_Patrol);

		//设置速度
		SECharacter->SetMaxSpeed(100.f);
	}
}

void ASlAiEnemyController::OnUnPossess()
{
	Super::OnUnPossess();

	//停止行为树
	if (BehaviorComp)
	{
		BehaviorComp->StopTree();
	}
}

FVector ASlAiEnemyController::GetPlayerLocation() const
{
	if (SPCharacter)
	{
		return SPCharacter->GetActorLocation();
	}
	return FVector::ZeroVector;
}

void ASlAiEnemyController::OnSeePlayer()
{
	if (IsLockPlayer || IsPlayerDead())
	{
		return;
	}

	IsLockPlayer = true;
	BlackboardComp->SetValueAsEnum("EnemyState",(uint8)EEnemyAIState::ES_Chase);
	SECharacter->SetMaxSpeed(300.f);
}

bool ASlAiEnemyController::IsPlayerDead()
{
	if (SPCharacter)
	{
		return SPCharacter->IsPlayerDead();
	}
	return false;
}

void ASlAiEnemyController::LoosePlayer()
{
	IsLockPlayer = false;
}

bool ASlAiEnemyController::IsPlayerAway()
{
	if (!IsLockPlayer || !SPCharacter || EPDisList.Num() < 6 || IsPlayerDead())
	{
		return false;
	}

	int BiggerNum = 0;
	float LastDis = -1.f;
	//有三个比前面的大，就判定远离
	for (TArray<float>::TIterator It(EPDisList); It; ++It)
	{
		if (*It > LastDis)
		{
			BiggerNum ++;
		}
		LastDis = *It;
	}
	return BiggerNum > 3;
}

UObject* ASlAiEnemyController::GetPlayerPawn()
{
	return SPCharacter;
}

void ASlAiEnemyController::ResetProcess(bool IsFinish)
{
	BlackboardComp->SetValueAsBool("ProcessFinish",IsFinish);
}

void ASlAiEnemyController::UpdateDamageRatio(float HPRatioVal)
{
	HPRatio = HPRatioVal;

	if (IsAllowHurt)
	{
		//修改为受伤
		BlackboardComp->SetValueAsEnum("EnemyState",(uint8)EEnemyAIState::ES_Hurt);
		IsAllowHurt = false;
	}
}

void ASlAiEnemyController::FinishStateHurt()
{
	//没有锁定玩家
	if (!IsLockPlayer)
	{
		IsLockPlayer = true;
	}

	//逃跑
	if (HPRatio < 0.2f)
	{
		BlackboardComp->SetValueAsEnum("EnemyState",(uint8)EEnemyAIState::ES_Escape);
	}
	else
	{
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int ActionRatio = Stream.RandRange(0,10);

		BlackboardComp->SetValueAsEnum("EnemyState",(uint8)EEnemyAIState::ES_Attack);
		
		//防御
		// if (ActionRatio < 4)
		// {
		// 	BlackboardComp->SetValueAsEnum("EnemyState",(uint8)EEnemyAIState::ES_Defence);
		// }
		// else
		// {
		// 	BlackboardComp->SetValueAsEnum("EnemyState",(uint8)EEnemyAIState::ES_Attack);
		// }
	}
}

