// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SlAiGameMode.generated.h"

//初始化背包管理
DECLARE_DELEGATE(FInitBagManger)
//注册小地图贴图
DECLARE_DELEGATE_OneParam(FRegisterMiniMap, class UTextureRenderTarget2D*)
//更新小地图
DECLARE_DELEGATE_FiveParams(FUpdateMapData, const FRotator, const float, const TArray<FVector2D>*, const TArray<bool>*, const TArray<float>*)

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
	
	FRegisterMiniMap RegisterMiniMap;
	
	FUpdateMapData UpdateMapData;
	
public:
	ASlAiGameMode();

	//重写帧函数
	virtual void Tick(float DeltaSeconds) override;

	//组件赋值
	void InitGameplayMode();

protected:
	
	void BeginPlay() override;

	void InitBagManager();

	void InitMiniMapCamera();

private:

	bool IsInitBag;

	bool IsInitMiniMap;
	//渲染小地图指针
	class ASlAiSceneCapture2D* MiniMapCamera;
};
