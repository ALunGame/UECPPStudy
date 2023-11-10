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
	// Sets default values for this actor's properties
	ASlAiEnemyController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	//玩家位置
	FVector GetPlayerLocation() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	class ASlAiPlayerCharacter* SPCharacter;
	
	class ASlAiEnemyCharacter* SECharacter;

	class UBlackboardComponent* BlackboardComp;

	class UBehaviorTreeComponent* BehaviorComp;
};
