// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlAiGameMode.generated.h"

//初始化背包管理
DECLARE_DELEGATE(FInitBagManger)

/**
 * 
 */
UCLASS()
class SLAICOURSE_API ASlAiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	class ASlAiPlayerController* SPController;
	class ASlAiPlayerCharacter* SPCharacter;
	class ASlAiPlayerState* SPState;

	FInitBagManger InitBagManger;
	
public:
	ASlAiGameMode();

	//重写帧函数
	virtual void Tick(float DeltaSeconds) override;

	//组件赋值
	void InitGameplayMode();

protected:
	
	void BeginPlay() override;

	void InitBagManager();

private:

	bool IsInitBag;
};
