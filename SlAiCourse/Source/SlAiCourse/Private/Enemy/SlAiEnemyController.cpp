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
	UBehaviorTree* StaticBehaviorTree = LoadObject<UBehaviorTree>(nullptr,TEXT("BehaviorTree'/Game/Blueprint/Enemy/EnemyBehaviorTree.EnemyBehaviorTree'"));
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

