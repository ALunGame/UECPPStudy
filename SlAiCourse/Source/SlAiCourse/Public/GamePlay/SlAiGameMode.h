// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlAiGameMode.generated.h"

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
	
public:
	ASlAiGameMode();

	//重写帧函数
	virtual void Tick(float DeltaSeconds) override;

	//组件赋值
	void InitGameplayMode();

protected:
	void BeginPlay() override;
};
