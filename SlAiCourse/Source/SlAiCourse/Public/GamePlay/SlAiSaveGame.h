// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SlAiSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USlAiSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	//玩家位置
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	FVector PlayerLocation;

	//玩家血量
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	float PlayerHP;

	//玩家饥饿
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	float PlayerHunger;

	//背包
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<int32> InputIndex;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<int32> InputNum;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<int32> NormalIndex;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<int32> NormalNum;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<int32> ShortcutIndex;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<int32> ShortcutNum;

	//敌人
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<FVector> EnemyLoaction;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<float> EnemyHP;

	//资源
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<FVector> ResourceRock;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<FVector> ResourceTree;

	//拾取物
	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<FVector> PickupStone;

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
	TArray<FVector> PickupWood;

};
