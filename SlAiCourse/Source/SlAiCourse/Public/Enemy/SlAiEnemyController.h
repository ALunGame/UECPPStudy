// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SlAiEnemyController.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiEnemyController : public AAIController
{
	GENERATED_BODY()

public:

	bool IsLockPlayer;

public:
	// Sets default values for this actor's properties
	ASlAiEnemyController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	//玩家位置
	FVector GetPlayerLocation() const;

	//看到玩家
	void OnSeePlayer();

	//看到玩家
	bool IsPlayerDead();

	void LoosePlayer();

	bool IsPlayerAway();
	
	UObject* GetPlayerPawn();

	//告诉控制器动作完成
	void ResetProcess(bool IsFinish);

	//传入剩余血量
	void UpdateDamageRatio(float HPRatioVal);

	//完成受伤状态
	void FinishStateHurt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	class ASlAiPlayerCharacter* SPCharacter;
	
	class ASlAiEnemyCharacter* SECharacter;

	class UBlackboardComponent* BlackboardComp;

	class UBehaviorTreeComponent* BehaviorComp;

	//和玩家距离
	TArray<float> EPDisList;

	FTimerHandle EPDisHandle;

	float HPRatio;

	//允许切换受伤状态
	bool IsAllowHurt;
	//受伤计时器
	float HurtTimeCnt;
private:

	void UpdateStateParam();
};
